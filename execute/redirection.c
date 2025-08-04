/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:55 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/04 12:19:25 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_in_fd(t_cmds *p, int x, int y, t_pipe *c)
{
	int	k;
	int	i;

	i = 0;
	k = 0;
	while (p[x].cmd[y])
	{
		i = 0;
		if (k != 0)
			write(c->fd1, " ", 1);
		while (p[x].cmd[y][i])
		{
			write(c->fd1, &p[x].cmd[y][i], 1);
			i++;
		}
		y++;
		k++;
	}
}

static int	open_read(t_pipe *c, int pm, t_cmds *p, int j)
{
	c->fd1 = open(p[j].outs[c->index].file_name, O_RDONLY, 0644);
	if (c->fd1 < 0)
	{
		perror("open");
		g_exit_code = 1;
		if (pm == 1 && c->character != 1)
			free_and_exit(c, p);
		else if (c->character == 1)
			exit_once(p, c);
		else
			return (-1);
	}
	dup2(c->fd1, STDIN_FILENO);
	return (0);
}

static int	open_out(t_pipe *c, int pm, t_cmds *p, int j)
{
	c->fd1 = open(p[j].outs[c->index].file_name, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (c->fd1 < 0)
	{
		perror("open");
		g_exit_code = 1;
		if (pm == 1 && c->character != 1)
			free_and_exit(c, p);
		else if (c->character == 1)
			exit_once(p, c);
		else
			return (-1);
	}
	dup2(c->fd1, STDOUT_FILENO);
	return (0);
}

static int	open_append(t_pipe *c, int pm, t_cmds *p, int j)
{
	c->fd1 = open(p[j].outs[c->index].file_name, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (c->fd1 < 0)
	{
		perror("open");
		g_exit_code = 1;
		if (pm == 1 && c->character != 1)
			free_and_exit(c, p);
		else if (c->character == 1)
			exit_once(p, c);
		else
			return (-1);
	}
	dup2(c->fd1, STDOUT_FILENO);
	return (0);
}

int	check_exec_redirect(t_cmds *p, t_pipe *c, int pm, int j)
{
	c->index = 0;
	c->fd1 = 0;
	while (c->index < p[j].redir_count)
	{
		if (p[j].outs[c->index].flag == 0)
		{
			if (open_read(c, pm, p, j) == -1)
				return (-1);
		}
		else if (p[j].outs[c->index].flag == 1)
		{
			if (open_out(c, pm, p, j) == -1)
				return (-1);
		}
		else if (p[j].outs[c->index].flag == 2)
		{
			if (open_append(c, pm, p, j) == -1)
				return (-1);
		}
		if (c->fd1 > 2)
			close(c->fd1);
		c->index++;
	}
	return (0);
}
