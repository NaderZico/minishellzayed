#include "execute.h"
#include <errno.h>
/*
 * apply_redirections
 *   - Loop over cmd->redirs[], open FDs, dup2() to STDIN/STDOUT, close originals.
 */
void apply_redirections(t_command *cmd)
{
    int fd;

    if (!cmd)
        return;

    if (cmd->pipe_in != -1)
    {   
        if (dup2(cmd->pipe_in, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(cmd->pipe_in);
            exit(ERR_GENERAL);
        }
        close(cmd->pipe_in);
    }

    for (int i = 0; i < cmd->redir_count; i++)
    {
        if (!cmd->redirs || !cmd->redirs[i].file)
        {
            // Don't print error here, just exit
            exit(ERR_GENERAL);
        }
        if (cmd->redirs[i].type == REDIR_IN)
        {
            fd = open(cmd->redirs[i].file, O_RDONLY);
            if (fd < 0)
            {
                // Don't print error here, just exit
                exit(ERR_GENERAL);
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(ERR_GENERAL);
            }
            close(fd);
        }
        else if (cmd->redirs[i].type == REDIR_OUT)
        {
            fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                // Don't print error here, just exit
                exit(ERR_GENERAL);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(ERR_GENERAL);
            }
            close(fd);
        }
        else if (cmd->redirs[i].type == REDIR_APPEND)
        {
            fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                // Don't print error here, just exit
                exit(ERR_GENERAL);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(ERR_GENERAL);
            }
            close(fd);
        }
        // REDIR_HEREDOC handled earlier with pipe, just ensure STDIN is already set
    }
}

// Only apply output redirections (for built-ins like echo)
void apply_output_redirections(t_command *cmd)
{
    int fd;
    if (!cmd)
        return;
    for (int i = 0; i < cmd->redir_count; i++)
    {
        if (!cmd->redirs || !cmd->redirs[i].file)
        {
            write(2, "minishell: redirection file is NULL\n", 36);
            exit(ERR_GENERAL);
        }
        if (cmd->redirs[i].type == REDIR_OUT)
        {
            fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror(cmd->redirs[i].file);
                exit(ERR_GENERAL);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(ERR_GENERAL);
            }
            close(fd);
        }
        else if (cmd->redirs[i].type == REDIR_APPEND)
        {
            fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(cmd->redirs[i].file);
                exit(ERR_GENERAL);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(ERR_GENERAL);
            }
            close(fd);
        }
        // Ignore input and heredoc for built-ins
    }
}

/*
 * close_all_pipes
 *   - Parent/child should close all pipe FDs after dup2.
 */
void close_all_pipes(int pipes[][2], int cmd_count)
{
    for (int i = 0; i < cmd_count - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

void    execute_piped_external(t_command    command, char *path,t_data    *data)
{
    if (!path)
    {
        if (command.args[0] && ft_strchr(command.args[0], '/')) {
            write(2, "minishell: ", 11);
            write(2, command.args[0], ft_strlen(command.args[0]));
            write(2, ": No such file or directory\n", 28);
        } else {
            write(2, "minishell: ", 11);
            write(2, command.args[0], ft_strlen(command.args[0]));
            write(2, ": command not found\n", 20);
        }
        exit(127);
    }
    if (path == (char *)-2)
    {
        write(2, "minishell: ", 11);
        write(2, command.args[0], ft_strlen(command.args[0]));
        write(2, ": is a directory\n", 17);
        exit(126);
    }
    if (path == (char *)-1)
    {
        write(2, command.args[0], ft_strlen(command.args[0]));
        write(2, ": Permission denied\n", 20);
        exit(126);
    }
    execve(path, command.args, data->env);
    if (errno == EACCES)
    {
        perror(command.args[0]);
        exit(126);
    }
    perror(command.args[0]);
    exit(127);
}

/*
 * launch_pipeline
 * ---------------
 * Executes a pipeline of commands (cmd1 | cmd2 | ... | cmdN).
 * 
 * - Pipes are created incrementally, not all at once, to avoid hitting system file descriptor limits.
 * - For each command:
 *     - If not the last command, create a pipe for its output.
 *     - Fork a child process.
 *         - In the child:
 *             - Set up STDIN from the previous pipe (if not first command).
 *             - Set up STDOUT to the current pipe (if not last command).
 *             - Close all unused pipe ends immediately.
 *             - Apply redirections as needed.
 *             - Execute the command (builtin or external).
 *         - In the parent:
 *             - Close the previous pipe ends (they are no longer needed).
 *             - Move the current pipe to be the "previous" for the next iteration.
 * - After all children are forked, parent closes any remaining pipe ends and waits for all children.
 * 
 * This approach matches bash's behavior: it minimizes the number of open pipes at any time,
 * allowing very large pipelines without hitting resource limits prematurely.
 */
static int precheck_all_redirections(t_data *data, int *redir_error)
{
    int i, r, fd;
    t_command *cmd;
    int error_found = 0;

    for (i = 0; i < data->cmd_count; i++) {
        cmd = &data->commands[i];
        redir_error[i] = 0;
        int stop_redir = 0;
        for (r = 0; r < cmd->redir_count; r++) {
            char *file = cmd->redirs[r].file;
            int type = cmd->redirs[r].type;
            if (!file || stop_redir)
                continue;
            if (type == REDIR_IN) {
                fd = open(file, O_RDONLY);
                if (fd < 0) {
                    write(2, "minishell: ", 11);
                    write(2, file, ft_strlen(file));
                    if (errno == EACCES)
                        write(2, ": Permission denied\n", 20);
                    else
                        write(2, ": No such file or directory\n", 28);
                    redir_error[i] = 1;
                    error_found = 1;
                    stop_redir = 1;
                } else {
                    close(fd);
                }
            } else if (type == REDIR_OUT) {
                if (!stop_redir) {
                    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0) {
                        write(2, "minishell: ", 11);
                        write(2, file, ft_strlen(file));
                        write(2, ": ", 2);
                        perror("");
                        error_found = 1;
                    } else {
                        close(fd);
                    }
                }
            } else if (type == REDIR_APPEND) {
                if (!stop_redir) {
                    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fd < 0) {
                        write(2, "minishell: ", 11);
                        write(2, file, ft_strlen(file));
                        write(2, ": ", 2);
                        perror("");
                        error_found = 1;
                    } else {
                        close(fd);
                    }
                }
            }
            // REDIR_HEREDOC: handled elsewhere
        }
    }
    return error_found ? 1 : 0;
}

int launch_pipeline(t_data *data, int *redir_error)
{
    if (!data || data->cmd_count <= 0)
        return (false);

    int stop_at = data->cmd_count;
    for (int i = 0; i < data->cmd_count; i++) {
        if (redir_error[i]) {
            stop_at = i;
            break;
        }
    }

    int prev_pipe[2] = {-1, -1};
    int curr_pipe[2] = {-1, -1};
    pid_t *pids = malloc(sizeof(pid_t) * stop_at);
    if (!pids)
    {
        free(redir_error);
        write(2, "minishell: malloc failed for pipeline\n", 38);
        data->last_status = 2;
        return (false);
    }

    // Ignore SIGINT/SIGQUIT in parent during fork
    struct sigaction sa_ignore, sa_orig_int, sa_orig_quit;
    sa_ignore.sa_handler = SIG_IGN;
    sigemptyset(&sa_ignore.sa_mask);
    sa_ignore.sa_flags = 0;
    sigaction(SIGINT, &sa_ignore, &sa_orig_int);
    sigaction(SIGQUIT, &sa_ignore, &sa_orig_quit);

    for (int i = 0; i < stop_at; i++)
    {
        // Create pipe for next command if not last
        if (i < stop_at - 1)
        {
            if (pipe(curr_pipe) == -1)
            {
                perror("minishell: pipe");
                data->last_status = 2;
                free(pids);
                free(redir_error);
                if (prev_pipe[0] != -1) close(prev_pipe[0]);
                if (prev_pipe[1] != -1) close(prev_pipe[1]);
                return (false);
            }
        }

        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("minishell: fork");
            data->last_status = 2;
            if (prev_pipe[0] != -1) close(prev_pipe[0]);
            if (prev_pipe[1] != -1) close(prev_pipe[1]);
            if (curr_pipe[0] != -1) close(curr_pipe[0]);
            if (curr_pipe[1] != -1) close(curr_pipe[1]);
            free(pids);
            free(redir_error);
            return (false);
        }
        else if (pids[i] == 0)
        {
            // Child: set up STDIN/STDOUT
            if (i > 0) {
                dup2(prev_pipe[0], STDIN_FILENO);
            } else if (data->commands[i].pipe_in != -1) {
                dup2(data->commands[i].pipe_in, STDIN_FILENO);
            }
            if (i < stop_at - 1) {
                dup2(curr_pipe[1], STDOUT_FILENO);
            }

            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            if (prev_pipe[0] != -1) close(prev_pipe[0]);
            if (prev_pipe[1] != -1) close(prev_pipe[1]);
            if (curr_pipe[0] != -1) close(curr_pipe[0]);
            if (curr_pipe[1] != -1) close(curr_pipe[1]);
            for (int j = 0; j < data->cmd_count; j++) {
                if (data->commands[j].pipe_in != -1)
                    close(data->commands[j].pipe_in);
            }

            apply_redirections(&data->commands[i]);

            if (isbuilt_in(data->commands[i]))
            {
                execute_builtin(data->commands[i], data);
                exit(data->last_status);
            }
            else
            {
                char *path = NULL;
                if (data->commands[i].args && data->commands[i].args[0])
                    path = find_command_path(data->commands[i].args[0], data->env);
                if (!path)
                {
                    if (data->commands[i].args && ft_strchr(data->commands[i].args[0], '/')) {
                        write(2, "minishell: ", 11);
                        write(2, data->commands[i].args[0], ft_strlen(data->commands[i].args[0]));
                        write(2, ": No such file or directory\n", 28);
                    } else if (data->commands[i].args) {
                        write(2, data->commands[i].args[0], ft_strlen(data->commands[i].args[0]));
                        write(2, ": command not found\n", 20);
                    }
                    exit(127);
                }
                if (path == (char *)-2)
                {
                    write(2, "minishell: ", 11);
                    write(2, data->commands[i].args[0], ft_strlen(data->commands[i].args[0]));
                    write(2, ": is a directory\n", 17);
                    exit(126);
                }
                if (path == (char *)-1)
                {
                    write(2, data->commands[i].args[0], ft_strlen(data->commands[i].args[0]));
                    write(2, ": Permission denied\n", 20);
                    exit(126);
                }
                execve(path, data->commands[i].args, data->env);
                if (errno == EACCES)
                {
                    perror(data->commands[i].args[0]);
                    exit(126);
                }
                perror("execve");
                exit(127);
            }
        }

        if (prev_pipe[0] != -1) close(prev_pipe[0]);
        if (prev_pipe[1] != -1) close(prev_pipe[1]);
        prev_pipe[0] = curr_pipe[0];
        prev_pipe[1] = curr_pipe[1];
        curr_pipe[0] = -1;
        curr_pipe[1] = -1;
    }

    if (prev_pipe[0] != -1) close(prev_pipe[0]);
    if (prev_pipe[1] != -1) close(prev_pipe[1]);
    for (int i = 0; i < data->cmd_count; i++) {
        if (data->commands[i].pipe_in != -1)
            close(data->commands[i].pipe_in);
    }

    sigaction(SIGINT, &sa_orig_int, NULL);
    sigaction(SIGQUIT, &sa_orig_quit, NULL);

    int status = 0;
    for (int i = 0; i < stop_at; i++)
    {
        int wstatus;
        waitpid(pids[i], &wstatus, 0);
        if (i == stop_at - 1)
            status = wstatus;
    }
    if (WIFEXITED(status))
        data->last_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        data->last_status = 128 + WTERMSIG(status);

    // Set exit status to 1 if any redirection error occurred
    if (redir_error)
        data->last_status = 1;

    free(pids);
    return (true);
}

void pipe_init(t_data *data)
{
    int i;
    for (i = 0; i < data->cmd_count; i++)
    {
        data->commands[i].pipe_in = -1;
        data->commands[i].pipe_out = -1;
    }
}

void execute_commands(t_data *data)
{
    if (!data || data->cmd_count <= 0)
        return;

    pipe_init(data);

    if (handle_heredocs(data) != 0)
    {
        data->last_status = ERR_GENERAL;
        return;
    }

    int *redir_error = calloc(data->cmd_count, sizeof(int));
    if (!redir_error)
    {
        data->last_status = 1;
        return;
    }

    int any_error = precheck_all_redirections(data, redir_error);

    if (data->cmd_count == 1)
    {
        t_command *cmd = &data->commands[0];

        if (isbuilt_in(*cmd) && (!cmd->args || !cmd->args[0] || cmd->redir_count == 0))
        {
            execute_builtin(*cmd, data);
        }
        else if (!redir_error[0])
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("fork");
                data->last_status = 1;
                free(redir_error);
                return;
            }
            if (pid == 0)
            {
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);

                apply_redirections(cmd);

                if (isbuilt_in(*cmd))
                {
                    execute_builtin(*cmd, data);
                    exit(data->last_status);
                }
                else
                {
                    char *path = NULL;
                    if (cmd->args && cmd->args[0])
                        path = find_command_path(cmd->args[0], data->env);
                    if (!path)
                    {
                        if (cmd->args && ft_strchr(cmd->args[0], '/')) {
                            ft_putstr_fd("minishell: ", 2);
                            ft_putstr_fd(cmd->args[0], 2);
                            ft_putstr_fd(": No such file or directory\n", 2);
                        } else if (cmd->args) {
                            ft_putstr_fd(cmd->args[0], 2);
                            ft_putstr_fd(": command not found\n", 2);
                        }
                        exit(127);
                    }
                    if (path == (char *)-2)
                    {
                        ft_putstr_fd("minishell: ", 2);
                        ft_putstr_fd(cmd->args[0], 2);
                        ft_putstr_fd(": is a directory\n", 2);
                        exit(126);
                    }
                    if (path == (char *)-1)
                    {
                        ft_putstr_fd(cmd->args[0], 2);
                        ft_putstr_fd(": Permission denied\n", 2);
                        exit(126);
                    }
                    execve(path, cmd->args, data->env);
                    if (errno == EACCES)
                    {
                        perror(cmd->args[0]);
                        exit(126);
                    }
                    perror(cmd->args[0]);
                    exit(127);
                }
            }
            else
            {
                signal(SIGINT, SIG_IGN);
                signal(SIGQUIT, SIG_IGN);

                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    data->last_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    data->last_status = 128 + WTERMSIG(status);
            }
        }
        // else: redirection error, do not execute
        if (any_error)
            data->last_status = 1;
    }
    else
    {
        launch_pipeline(data, redir_error);
    }

    free(redir_error);
}
