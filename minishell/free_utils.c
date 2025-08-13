/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:52:44 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 16:52:44 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strings(char **av)
{
	int	i;

	i = 0;
	while (av && av[i])
	{
		free(av[i]);
		av[i] = NULL;
		i++;
	}
	if (av)
	{
		free(av);
		av = NULL;
	}
}

static void	free_condition(int i, int j, t_cmds *cmd)
{
	free(cmd[i].outs[j].file_name);
	cmd[i].outs[j].file_name = NULL;
}

void	free_all(t_pipe *pipe, t_cmds *cmd)
{
	pipe->index = -1;
	pipe->loop_counter = -1;
	while (++pipe->index < pipe->total_cmds)
	{
		pipe->loop_counter = -1;
		if (cmd[pipe->index].red_len > 0)
		{
			while (++pipe->loop_counter < cmd[pipe->index].red_len)
				if (cmd[pipe->index].outs[pipe->loop_counter].file_name)
					free_condition(pipe->index, pipe->loop_counter, cmd);
			if (cmd[pipe->index].outs)
			{
				free(cmd[pipe->index].outs);
				cmd[pipe->index].outs = NULL;
			}
		}
		free_strings(cmd[pipe->index].cmd);
	}
	free_strings(pipe->cmds);
	pipe->cmds = NULL;
	free(cmd);
	cmd = NULL;
}
