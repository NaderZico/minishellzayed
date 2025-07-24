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
            write(2, "minishell: redirection file is NULL\n", 36);
            exit(ERR_GENERAL);
        }
        if (cmd->redirs[i].type == REDIR_IN)
        {
            fd = open(cmd->redirs[i].file, O_RDONLY);
            if (fd < 0)
            {
                perror(cmd->redirs[i].file);
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
            write(2, command.args[0], ft_strlen(command.args[0]));
            write(2, ": command not found\n", 20);
        }
        exit(127);
    }
    if (path == (char *)-2)
    {
        write(2, "minishell: ", 11);
        write(2, command.args[0], ft_strlen(command.args[0]));
        write(2, ": Permission denied\n", 20);
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


int launch_pipeline(t_data *data)
{
    if (!data || data->cmd_count <= 0)
        return (false);
    int pipes[data->cmd_count - 1][2];
    pid_t *pids = malloc(sizeof(pid_t) * data->cmd_count);
    if (!pids)
        return (false);

    // Create all pipes first
    for (int i = 0; i < data->cmd_count - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < data->cmd_count; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pids[i] == 0)
        {
            // Child: setup STDIN/STDOUT from pipes/heredoc
            if (data->commands[i].pipe_in != -1) {
                dup2(data->commands[i].pipe_in, STDIN_FILENO);
            } else if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < data->cmd_count - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe fds in child
            for (int j = 0; j < data->cmd_count - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            // Close heredoc pipes in child
            for (int j = 0; j < data->cmd_count; j++) {
                if (data->commands[j].pipe_in != -1)
                    close(data->commands[j].pipe_in);
            }

            // Now apply only file redirections that do NOT conflict with pipes/heredoc
            for (int r = 0; r < data->commands[i].redir_count; r++) {
                int type = data->commands[i].redirs[r].type;
                char *file = data->commands[i].redirs[r].file;
                int fd;
                if (!file)
                    continue;
                // Apply input redirection for first command if not set by heredoc/pipe
                if (type == REDIR_IN && data->commands[i].pipe_in == -1 && i == 0) {
                    fd = open(file, O_RDONLY);
                    if (fd < 0) { perror(file); exit(ERR_GENERAL); }
                    if (dup2(fd, STDIN_FILENO) == -1) { perror("dup2"); close(fd); exit(ERR_GENERAL); }
                    close(fd);
                }
                // Only apply output redirection if STDOUT not already set by pipe
                if ((type == REDIR_OUT || type == REDIR_APPEND) && i == data->cmd_count - 1) {
                    int flags = (type == REDIR_OUT) ? (O_WRONLY | O_CREAT | O_TRUNC) : (O_WRONLY | O_CREAT | O_APPEND);
                    fd = open(file, flags, 0644);
                    if (fd < 0) { perror(file); exit(ERR_GENERAL); }
                    if (dup2(fd, STDOUT_FILENO) == -1) { perror("dup2"); close(fd); exit(ERR_GENERAL); }
                    close(fd);
                }
            }

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
                    write(2, ": Permission denied\n", 20);
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
    }

    // Parent: close all pipe ends after forking
    for (int i = 0; i < data->cmd_count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    // Parent: close heredoc pipes
    for (int i = 0; i < data->cmd_count; i++) {
        if (data->commands[i].pipe_in != -1)
            close(data->commands[i].pipe_in);
    }

    // Wait for children
    for (int i = 0; i < data->cmd_count; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
        if (i == data->cmd_count - 1)
        {
            if (WIFEXITED(status))
                data->last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                data->last_status = 128 + WTERMSIG(status);
        }
    }
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

	if (data->cmd_count == 1)
	{
		t_command *cmd = &data->commands[0];

		if (isbuilt_in(*cmd) && (!cmd->args || !cmd->args[0] || cmd->redir_count == 0))
		{
			// Built-in with no redirection: safe to run in parent
			execute_builtin(*cmd, data);
		}
		else
		{
			pid_t pid = fork();
			if (pid < 0)
			{
				perror("fork");
				data->last_status = 1;
				return;
			}
			if (pid == 0)
			{
				// Child process
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
						ft_putstr_fd(": Permission denied\n", 2);
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
				// Parent process
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
	}
	else
	{
		launch_pipeline(data);
	}
}
