/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:01:00 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 17:01:00 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_list(t_list **lst)
{
	t_list	*tmp;

	while ((*lst)->next)
	{
		tmp = *lst;
		(*lst) = (*lst)->next;
		free(tmp->content);
		free(tmp);
	}
	free((*lst)->content);
	free(*lst);
}

void	free_and_exit(t_pipe *pipe, t_cmds *cmds)
{
	if (pipe->e_fd > 2)
		close(pipe->e_fd);
	if (pipe->env_variables)
		free_list(&pipe->env_variables);
	if (pipe->m_export)
		free_list(&pipe->m_export);
	if (pipe->tmp_env)
		free_strings(pipe->tmp_env);
	if (pipe->m_path != NULL)
		free_strings(pipe->m_path);
	free_all(pipe, cmds);
	check_and_exit(pipe);
	close(pipe->fdin);
	close(pipe->fdout);
	exit(g_exit_code);
}

void	free_and_exit_2(t_pipe *pipe_data, t_cmds *commandPtr)
{
	(void)commandPtr;
	if (pipe_data->env_variables)
		free_list(&pipe_data->env_variables);
	if (pipe_data->m_export)
		free_list(&pipe_data->m_export);
	if (pipe_data->tmp_env)
		free_strings(pipe_data->tmp_env);
	if (pipe_data->m_path != NULL)
		free_strings(pipe_data->m_path);
	close(pipe_data->fdout);
	close(pipe_data->fdin);
}

int	check_exit(t_cmds *cmds, int exit_code)
{
	int	i;

	i = 0;
	if (cmds[0].cmd[1])
	{
		if (exit_code == 0)
		{
			while (cmds[0].cmd[1][i])
			{
				if (!ft_isalpha(cmds[0].cmd[1][i]))
					return (1);
				i++;
			}
		}
		else
		{
			if (ft_atoi(cmds[0].cmd[1]) > INT_MAX
				|| ft_atoi(cmds[0].cmd[1]) < INT_MIN)
				return (1);
		}
	}
	return (0);
}

void	ft_exit(t_pipe *pipe, t_cmds *cmds)
{
	if (cmds[0].cmd[1])
	{
		if (get_exit_code(cmds) == 0)
			return ;
	}
	if (pipe->env_variables)
		free_list(&pipe->env_variables);
	if (pipe->m_export)
		free_list(&pipe->m_export);
	if (pipe->tmp_env)
		free_strings(pipe->tmp_env);
	if (pipe->m_path != NULL)
		free_strings(pipe->m_path);
	close(pipe->fdin);
	close(pipe->fdout);
	free_all(pipe, cmds);
	exit(g_exit_code);
}
