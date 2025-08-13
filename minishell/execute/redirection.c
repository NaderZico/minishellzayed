/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:55 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:52:55 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_in_fd(t_cmds *cmd, int x, int y, t_pipe *pipe)
{
	int	k;
	int	i;

	i = 0;
	k = 0;
	while (cmd[x].cmd[y])
	{
		i = 0;
		if (k != 0)
			write(pipe->fd1, " ", 1);
		while (cmd[x].cmd[y][i])
		{
			write(pipe->fd1, &cmd[x].cmd[y][i], 1);
			i++;
		}
		y++;
		k++;
	}
}

static int	open_read(t_pipe *pipe, int pipe_mode, t_cmds *cmd, int cmd_fd)
{
	pipe->fd1 = open(cmd[cmd_fd].outs[pipe->index].file_name, O_RDONLY, 0644);
	if (pipe->fd1 < 0)
	{
		perror("open");
		g_exit_code = 1;
		if (pipe_mode == 1 && pipe->character != 1)
			free_and_exit(pipe, cmd);
		else if (pipe->character == 1)
			exit_once(cmd, pipe);
		else
			return (-1);
	}
	dup2(pipe->fd1, STDIN_FILENO);
	return (0);
}

static int	open_out(t_pipe *pipe, int pipe_mode, t_cmds *cmd, int cmd_fd)
{
	pipe->fd1 = open(cmd[cmd_fd].outs[pipe->index].file_name,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipe->fd1 < 0)
	{
		perror("open");
		g_exit_code = 1;
		if (pipe_mode == 1 && pipe->character != 1)
			free_and_exit(pipe, cmd);
		else if (pipe->character == 1)
			exit_once(cmd, pipe);
		else
			return (-1);
	}
	dup2(pipe->fd1, STDOUT_FILENO);
	return (0);
}

static int	open_append(t_pipe *pipe, int pipe_mode, t_cmds *cmd, int cmd_fd)
{
	pipe->fd1 = open(cmd[cmd_fd].outs[pipe->index].file_name,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (pipe->fd1 < 0)
	{
		perror("open");
		g_exit_code = 1;
		if (pipe_mode == 1 && pipe->character != 1)
			free_and_exit(pipe, cmd);
		else if (pipe->character == 1)
			exit_once(cmd, pipe);
		else
			return (-1);
	}
	dup2(pipe->fd1, STDOUT_FILENO);
	return (0);
}

int	check_exec_redirect(t_cmds *cmd, t_pipe *pipe, int pipe_mode, int cmd_fd)
{
	pipe->index = 0;
	pipe->fd1 = 0;
	while (pipe->index < cmd[cmd_fd].red_len)
	{
		if (cmd[cmd_fd].outs[pipe->index].flag == 0)
		{
			if (open_read(pipe, pipe_mode, cmd, cmd_fd) == -1)
				return (-1);
		}
		else if (cmd[cmd_fd].outs[pipe->index].flag == 1)
		{
			if (open_out(pipe, pipe_mode, cmd, cmd_fd) == -1)
				return (-1);
		}
		else if (cmd[cmd_fd].outs[pipe->index].flag == 2)
		{
			if (open_append(pipe, pipe_mode, cmd, cmd_fd) == -1)
				return (-1);
		}
		if (pipe->fd1 > 2)
			close(pipe->fd1);
		pipe->index++;
	}
	return (0);
}
