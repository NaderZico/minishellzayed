/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:15 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:53:30 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	closing_fds(t_pipe *pipe)
{
	if (pipe->pipe_flag_read == 1)
	{
		close(pipe->pipe_fds[0][0]);
		close(pipe->pipe_fds[0][1]);
	}
	if (pipe->pipe_flag_read_index == 1)
	{
		close(pipe->pipe_fds[1][0]);
		close(pipe->pipe_fds[1][1]);
	}
}

void	ft_unset_p(t_cmds *cmds, int i, int fd, t_pipe *pipe)
{
	int	j;

	j = 0;
	(void)fd;
	if (!cmds[i].cmd[j + 1])
		return ;
	j = 1;
	while (cmds[i].cmd[j])
	{
		if (ft_isalpha_str(cmds[i].cmd[j]) == 0)
		{
			unset_cmp(&pipe->env_variables, cmds[i].cmd[j], pipe);
			unset_cmp(&pipe->m_export, cmds[i].cmd[j], pipe);
		}
		else
			print_error(i, j, cmds);
		j++;
	}
	pipe->env_count -= 1;
}

void	finalize_middle_command(t_pipe *pipe, t_cmds *cmds, t_vars *v)
{
	close(pipe->e_fd);
	closing_fds(pipe);
	if (builtins_pipes(cmds, pipe, pipe->pipe_fds[0][1], v->counter) == 0)
		free_and_exit(pipe, cmds);
	pipe->cmd_exec = check_command_existence(cmds[v->counter].cmd[0],
			pipe->m_path);
	if (!pipe->cmd_exec)
		child_exit(cmds, v->counter, pipe, 0);
	if (execve(pipe->cmd_exec, cmds[v->counter].cmd, NULL) < 0)
	{
		write(2, cmds[v->counter].cmd[0], ft_strlen(cmds[v->counter].cmd[0]));
		write(2, ": command not found\n", 21);
		free(pipe->cmd_exec);
		g_exit_code = 126;
		free_and_exit(pipe, cmds);
	}
}

void	closing_pipe(t_pipe *pipe, t_cmds *cmds, t_vars *v)
{
	if (v->i % 2 == 1 && v->i != 0)
	{
		close(pipe->pipe_fds[0][0]);
		close(pipe->pipe_fds[0][1]);
		if (v->counter == cmds->cmd_len - 1)
		{
			close(pipe->pipe_fds[1][1]);
			close(pipe->pipe_fds[1][0]);
		}
		v->i = -1;
		pipe->pipe_flag_read = 0;
	}
	else if (v->i % 2 == 0 && v->counter != 0)
	{
		close(pipe->pipe_fds[1][0]);
		close(pipe->pipe_fds[1][1]);
		v->i = 0;
		pipe->pipe_flag_read_index = 0;
	}
}

void	export_last(t_pipe *pipe, t_cmds *cmds, int j)
{
	ft_lstadd_back(&pipe->m_export, ft_lstnew(ft_strdup(cmds[0].cmd[j])));
	ft_lstadd_back(&pipe->env_variables, ft_lstnew(ft_strdup(cmds[0].cmd[j])));
	pipe->env_count += 1;
}
