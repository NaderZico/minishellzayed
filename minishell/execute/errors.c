/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:01:17 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 07:51:32 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execve_error(t_cmds *cmds, t_pipe *pipe)
{
	perror("execve : is directory");
	g_exit_code = 126;
	free(pipe->cmd_exec);
	free_and_exit(pipe, cmds);
}

void	exit_status(t_pipe *pipe)
{
	if (WIFEXITED(pipe->status))
		g_exit_code = WEXITSTATUS(pipe->status);
	else if (WIFSIGNALED(pipe->status))
		g_exit_code = WTERMSIG(pipe->status) + 128;
	free(pipe->cmd_exec);
	pipe->cmd_exec = NULL;
	pipe->character = 0;
}

int	check_if_file(t_cmds *cmds)
{
	struct stat	fs;

	if (stat(cmds[0].cmd[0], &fs) != 0)
	{
		perror("stat");
		g_exit_code = 126;
		return (1);
	}
	return (0);
}

int	check_dir(t_cmds *cmds)
{
	struct stat	fs;

	if (stat(cmds[0].cmd[0], &fs) != 0)
	{
		perror("stat");
		g_exit_code = 127;
		return (1);
	}
	return (0);
}

void	export_add(t_pipe *pipe, int i, int j, t_cmds *cmds)
{
	pipe->tmp2 = ft_lstnew(ft_strdup(cmds[0].cmd[j]));
	pipe->tmp2->next = pipe->tmp1;
	if (i != 0)
		pipe->tmp3->next = pipe->tmp2;
	pipe->env_count += 1;
	ft_lstadd_back(&pipe->env_variables, ft_lstnew(ft_strdup(cmds[0].cmd[j])));
	if (i == 0)
		pipe->m_export = pipe->tmp2;
}
