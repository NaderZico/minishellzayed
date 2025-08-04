/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:16:09 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/04 12:19:25 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_heredoc(t_cmds *p, t_pipe *c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	c->fd1 = 0;
	while (j < p->cmd_len)
	{
		i = 0;
		while (i < p[j].redir_count)
		{
			if (p[j].outs[i].flag == 3)
				return (1);
			i++;
		}
		j++;
	}
	return (0);
}

static int	open_file(t_vars *vars, t_pipe *pipeControl, t_cmds *p)
{
	vars->temp_buffer = create_file(pipeControl);
	vars->tmp = open(vars->temp_buffer, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (vars->tmp < 0)
	{
		free(vars->temp_buffer);
		pipeControl->debug_timing_flag = pipeControl->debug_timing_flag * 2 + 1;
		vars->temp_buffer = create_file(pipeControl);
		vars->tmp = open(vars->temp_buffer, O_RDWR | O_TRUNC | O_CREAT, 0644);
		if (vars->tmp < 0)
		{
			free(vars->temp_buffer);
			free_all(pipeControl, p);
			return (1);
		}
	}
	return (0);
}

void	break_condition(t_cmds *cmd_list, int i, t_vars *vars)
{
	cmd_list[i].outs[vars->redir_i].flag = 0;
	free(cmd_list[i].outs[vars->redir_i].file_name);
	cmd_list[i].outs[vars->redir_i].file_name = vars->temp_buffer;
	free(vars->current_quote);
}

static void	init_heredoc(t_pipe *c, t_vars *v)
{
	c->fd1 = 0;
	v->current_quote = NULL;
	v->temp_buffer = NULL;
	v->redir_i = -1;
	c->debug_timing_flag = 0;
	v->tmp = 0;
}

int	exec_heredoc(t_cmds *command, t_pipe *commandPipe, int commandIndex)
{
	t_vars	variables;

	init_heredoc(commandPipe, &variables);
	while (++variables.redir_i < command[commandIndex].redir_count)
	{
		if (command[commandIndex].outs[variables.redir_i].flag == 3)
		{
			signal(SIGINT, SIG_IGN);
			if (open_file(&variables, commandPipe, command) == 1)
				return (0);
			write(1, "> ", 2);
			variables.line = get_next_line(0);
			if (!variables.line)
				return (free(variables.temp_buffer), close(variables.tmp), 1);
			dollar_expansion(&variables.line, commandPipe);
			loop_heredoc(commandPipe, command, &variables, commandIndex);
			if (variables.redir_i == command[commandIndex].redir_count - 1)
				break ;
			if (variables.tmp > 2)
				close(variables.tmp);
		}
	}
	if (variables.tmp > 2)
		close(variables.tmp);
	return (0);
}
