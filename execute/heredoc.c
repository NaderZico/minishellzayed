#include "execute.h"

/*
 * handle_heredocs
 *   Iterate all commands’ redirs[], for each REDIR_HEREDOC:
 *     • create pipe
 *     • read lines until delimiter
 *     • write to pipe[1], close write end
 *     • store pipe[0] in a new cmd->heredoc_fd field
 */ 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>


#include "minishell.h"

int	found_heredoc(t_command *cmd, t_data *data, char *limiter)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	char	*line;

	if (pipe(fd) == -1)
		return (ERR_GENERAL);

	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (ERR_GENERAL);
	}

	if (pid == 0)
	{
		// --- CHILD: read input and write to pipe ---
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(fd[0]);

		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				ft_putstr_fd("minishell: warning: heredoc delimited by EOF\n", 2);
				break;
			}
			if (ft_strcmp(line, limiter) == 0)
			{
				free(line);
				break;
			}
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
		}
		close(fd[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		// --- PARENT: wait and store read end ---
		close(fd[1]);
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, SIG_DFL);

		if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
		{
			close(fd[0]);
			cmd->pipe_in = -1;
			data->last_status = WIFSIGNALED(status)
				? 128 + WTERMSIG(status)
				: WEXITSTATUS(status);
			return (data->last_status);
		}
		cmd->pipe_in = fd[0]; // used as STDIN in execution
	}
	return (SUCCESS);
}



#include "minishell.h"

int	handle_heredocs(t_data *data)
{
	int	i, j;

	if (!data || data->cmd_count == 0)
		return (SUCCESS);

	i = 0;
	while (i < data->cmd_count)
	{
		j = 0;
		while (j < data->commands[i].redir_count)
		{
			if (data->commands[i].redirs[j].type == REDIR_HEREDOC)
			{
				data->last_status = found_heredoc(
					&data->commands[i],
					data,
					data->commands[i].redirs[j].file);
				if (data->last_status != SUCCESS)
					return (data->last_status);
			}
			j++;
		}
		i++;
	}
	return (SUCCESS);
}



