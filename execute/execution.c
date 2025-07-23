#include "execute.h"

// typedef enum e_error
// {
// 	SUCCESS = 0,
// 	ERR_GENERAL = 1,             // General error (e.g., cd: too many args)
// 	ERR_SYNTAX = 2,              // Syntax error (like unexpected token)
// 	ERR_CMD_PERMISSION = 126,    // Command found but permission denied
// 	ERR_CMD_NOT_FOUND = 127,     // Command not found
// 	ERR_SIGINT = 130,            // Ctrl-C (SIGINT)
// 	ERR_SIGQUIT = 131,           // Ctrl-\ (SIGQUIT)
// 	ERR_MALLOC = 255             // Internal error (custom)
// } t_error;
/*
 * apply_redirections
 *   - Loop over cmd->redirs[], open FDs, dup2() to STDIN/STDOUT, close originals.
 */
void apply_redirections(t_command *cmd)
{
    int fd;

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
        if (!cmd->redirs[i].file)
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
                exit(ERR_CMD_PERMISSION);
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
                exit(ERR_CMD_PERMISSION);
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
                exit(ERR_CMD_PERMISSION);
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
    for (int i = 0; i < cmd->redir_count; i++)
    {
        if (!cmd->redirs[i].file)
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
                exit(ERR_CMD_PERMISSION);
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
                exit(ERR_CMD_PERMISSION);
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

// /*
//  * apply_pipes
//  *   - Given index i and pipes[][] from the parent,
//  *     dup2 read end to STDIN and/or write end to STDOUT.
//  */
// void apply_pipes(int i, int pipes[][2], int cmd_count)
// {
//     // TODO
// }

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
			write(2, command.args[0], ft_strlen(command.args[0]));
			write(2, ": command not found\n", 20);
			exit(127);
		}
		execve(path, command.args, data->env);
		perror(command.args[0]);
		exit(127);
}


int launch_pipeline(t_data *data)
{
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
                // Only apply input redirection if STDIN not already set by heredoc/pipe
                if (type == REDIR_IN && data->commands[i].pipe_in == -1 && i == 0 && data->cmd_count == 1) {
                    // Only for single command, not pipeline
                    fd = open(file, O_RDONLY);
                    if (fd < 0) { perror(file); exit(ERR_CMD_PERMISSION); }
                    if (dup2(fd, STDIN_FILENO) == -1) { perror("dup2"); close(fd); exit(ERR_GENERAL); }
                    close(fd);
                }
                // Only apply output redirection if STDOUT not already set by pipe
                if ((type == REDIR_OUT || type == REDIR_APPEND) && i == data->cmd_count - 1) {
                    int flags = (type == REDIR_OUT) ? (O_WRONLY | O_CREAT | O_TRUNC) : (O_WRONLY | O_CREAT | O_APPEND);
                    fd = open(file, flags, 0644);
                    if (fd < 0) { perror(file); exit(ERR_CMD_PERMISSION); }
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
                char *path = find_command_path(data->commands[i].args[0], data->env);
                if (!path)
                {
                    write(2, data->commands[i].args[0], ft_strlen(data->commands[i].args[0]));
                    write(2, ": command not found\n", 20);
                    exit(127);
                }
                execve(path, data->commands[i].args, data->env);
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



#include "minishell.h"

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

		if (isbuilt_in(*cmd) && cmd->redir_count == 0)
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
					char *path = find_command_path(cmd->args[0], data->env);
					if (!path)
					{
						ft_putstr_fd(cmd->args[0], 2);
						ft_putstr_fd(": command not found\n", 2);
						exit(127);
					}
					execve(path, cmd->args, data->env);
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
