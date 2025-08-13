/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:40 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/13 13:29:41 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_heredoc(t_cmds *cmds, t_pipe *pipe)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	pipe->fd1 = 0;
	while (j < cmds->cmd_len)
	{
		i = 0;
		while (i < cmds[j].red_len)
		{
			if (cmds[j].outs[i].flag == 3)
				return (1);
			i++;
		}
		j++;
	}
	return (0);
}

static int	open_file(t_vars *vars, t_pipe *pipe, t_cmds *cmds)
{
	vars->temp_buffer = create_file(pipe);
	vars->tmp = open(vars->temp_buffer, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (vars->tmp < 0)
	{
		free(vars->temp_buffer);
		pipe->debug_timing_flag = pipe->debug_timing_flag * 2 + 1;
		vars->temp_buffer = create_file(pipe);
		vars->tmp = open(vars->temp_buffer, O_RDWR | O_TRUNC | O_CREAT, 0644);
		if (vars->tmp < 0)
		{
			free(vars->temp_buffer);
			free_all(pipe, cmds);
			return (1);
		}
	}
	return (0);
}

void	break_condition(t_cmds *cmd_list, int i, t_vars *vars)
{
	cmd_list[i].outs[vars->redirect_index].flag = 0;
	free(cmd_list[i].outs[vars->redirect_index].file_name);
	cmd_list[i].outs[vars->redirect_index].file_name = vars->temp_buffer;
	free(vars->current_quote);
}

static void	init_heredoc(t_pipe *pipe, t_vars *v)
{
	pipe->fd1 = 0;
	v->current_quote = NULL;
	v->temp_buffer = NULL;
	v->redirect_index = -1;
	pipe->debug_timing_flag = 0;
	v->tmp = 0;
}

int	exec_heredoc(t_cmds *command, t_pipe *commandPipe, int commandIndex)
{
	t_vars	variables;

	init_heredoc(commandPipe, &variables);
	while (++variables.redirect_index < command[commandIndex].red_len)
	{
		if (command[commandIndex].outs[variables.redirect_index].flag == 3)
		{
			signal(SIGINT, SIG_IGN);
			if (open_file(&variables, commandPipe, command) == 1)
				return (0);
			write(1, "> ", 2);
			variables.line = get_next_line(0);
			if (!variables.line)
				return (free(variables.temp_buffer), close(variables.tmp), 1);
			expand_dollar_vars(&variables.line, commandPipe);
			loop_heredoc(commandPipe, command, &variables, commandIndex);
			if (variables.redirect_index == command[commandIndex].red_len - 1)
				break ;
			if (variables.tmp > 2)
				close(variables.tmp);
		}
	}
	if (variables.tmp > 2)
		close(variables.tmp);
	return (0);
}
