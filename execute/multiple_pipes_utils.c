/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 23:46:46 by abdamoha          #+#    #+#             */
/*   Updated: 2025/07/30 16:11:08 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	closing_fds(t_pipe *c)
{
	if (c->pipe_flag_read == 1)
	{
		close(c->pipe_fds[0][0]);
		close(c->pipe_fds[0][1]);
	}
	if (c->pipe_flag_read_index == 1)
	{
		close(c->pipe_fds[1][0]);
		close(c->pipe_fds[1][1]);
	}
}

void	ft_unset_p(t_cmds *p, int i, int fd, t_pipe *c)
{
	int	j;

	j = 0;
	(void)fd;
	if (!p[i].cmd[j + 1])
		return ;
	j = 1;
	while (p[i].cmd[j])
	{
		if (ft_isalpha_str(p[i].cmd[j]) == 0)
		{
			unset_cmp(&c->env_variables, p[i].cmd[j], c);
			unset_cmp(&c->m_export, p[i].cmd[j], c);
		}
		else
			print_error(i, j, p);
		j++;
	}
	c->env_count -= 1;
}

void	sixth_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	close(c->e_fd);
	closing_fds(c);
	if (builtins_pipes(p, c, c->pipe_fds[0][1], v->counter) == 0)
		free_and_exit(c, p);
	c->cmd_exec = check_command_existence(p[v->counter].cmd[0], c->m_path);
	if (!c->cmd_exec)
		child_exit(p, v->counter, c, 0);
	if (execve(c->cmd_exec, p[v->counter].cmd, NULL) < 0)
	{
		write(2, p[v->counter].cmd[0],
			ft_strlen(p[v->counter].cmd[0]));
		write(2, ": command not found\n", 21);
		free(c->cmd_exec);
		g_exit_code = 126;
		free_and_exit(c, p);
	}
}

void	closing_pipe(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (v->i % 2 == 1 && v->i != 0)
	{
		close(c->pipe_fds[0][0]);
		close(c->pipe_fds[0][1]);
		if (v->counter == p->cmd_len - 1)
		{
			close(c->pipe_fds[1][1]);
			close(c->pipe_fds[1][0]);
		}
		v->i = -1;
		c->pipe_flag_read = 0;
	}
	else if (v->i % 2 == 0 && v->counter != 0)
	{
		close(c->pipe_fds[1][0]);
		close(c->pipe_fds[1][1]);
		v->i = 0;
		c->pipe_flag_read_index = 0;
	}
}

void	export_last(t_pipe *c, t_cmds *p, int j)
{
	ft_lstadd_back(&c->m_export, ft_lstnew(ft_strdup(p[0].cmd[j])));
	ft_lstadd_back(&c->env_variables, ft_lstnew(ft_strdup(p[0].cmd[j])));
	c->env_count += 1;
}
