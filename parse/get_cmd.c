/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:00:34 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/04 16:20:27 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_quote_state(char c, int *quote_state)
{
	if (c == '\'' || c == '\"')
	{
		if (*quote_state == 0)
			*quote_state = c;
		else if (*quote_state == c)
			*quote_state = 0;
	}
}

void	store_file_name(char *str, char **file_name, int start_index,
		t_vars *vars)
{
	int	end_index;
	int	in_quotes;

	end_index = start_index;
	in_quotes = 0;
	while (str[end_index])
	{
		update_quote_state(str[end_index], &in_quotes);
		if ((str[end_index] == ' ' || str[end_index] == '\t') && !in_quotes)
			break ;
		end_index++;
	}
	vars->i = end_index;
	*file_name = ft_substr(str, start_index, end_index - start_index);
}

void	fill_files(t_pipe *pipe, t_cmds *cmds, t_vars *vars)
{
	vars->start = vars->exit_status - 1;
	if (pipe->cmds[vars->counter][vars->exit_status + 1] == '>'
		|| pipe->cmds[vars->counter][vars->exit_status + 1] == '<')
	{
		if (pipe->cmds[vars->counter][vars->exit_status + 1] == '>')
			cmds[vars->counter].outs[vars->exit_code].flag = APPEND;
		else if (pipe->cmds[vars->counter][vars->exit_status + 1] == '<')
			cmds[vars->counter].outs[vars->exit_code].flag = HERE_DOC;
		vars->exit_status += 2;
	}
	else if (pipe->cmds[vars->counter][vars->exit_status] == '>')
	{
		cmds[vars->counter].outs[vars->exit_code].flag = OUT_FILE;
		vars->exit_status++;
	}
	else if (pipe->cmds[vars->counter][vars->exit_status] == '<')
	{
		cmds[vars->counter].outs[vars->exit_code].flag = IN_FILE;
		vars->exit_status++;
	}
}

void	save_redirections(t_pipe *pipe, t_cmds *cmds, t_vars *vars)
{
	vars->exit_code = 0;
	vars->exit_status = -1;
	while (pipe->cmds[vars->counter][++vars->exit_status])
	{
		update_quote_state(pipe->cmds[vars->counter][vars->exit_status],
			&vars->quote_char);
		if ((pipe->cmds[vars->counter][vars->exit_status] == '>'
			|| pipe->cmds[vars->counter][vars->exit_status] == '<')
			&& !vars->quote_char)
		{
			fill_files(pipe, cmds, vars);
			store_file_name(pipe->cmds[vars->counter],
				&cmds[vars->counter].outs[vars->exit_code].file_name,
				vars->exit_status + 1, vars);
			clean_quotes(cmds[vars->counter].outs[vars->exit_code].file_name);
			remove_substr(pipe->cmds[vars->counter], vars->start, vars->i);
			vars->exit_status = vars->start - 1;
			vars->exit_code++;
		}
	}
}

void	files_saving(t_pipe *pipe, t_cmds **cmds_ptr)
{
	t_cmds	*cmds;
	t_vars	vars;

	vars.start = 0;
	vars.quote_char = 0;
	vars.helper_i = 0;
	vars.counter = -1;
	vars.exit_status = 0;
	*cmds_ptr = malloc(sizeof(t_cmds) * pipe->total_cmds);
	cmds = *cmds_ptr;
	cmds->cmd_len = pipe->total_cmds;
	cmds->redir_count = 0;
	while (++vars.counter < pipe->total_cmds)
	{
		cmds[vars.counter].redir_count = count_redirs(pipe->cmds[vars.counter]);
		if (cmds[vars.counter].redir_count)
			cmds[vars.counter].outs = malloc(sizeof(t_redirect)
					* cmds[vars.counter].redir_count);
		save_redirections(pipe, cmds, &vars);
		cmds[vars.counter].cmd = ft_split(pipe->cmds[vars.counter], ' ');
		vars.helper_i = 0;
		while (cmds[vars.counter].cmd[vars.helper_i])
			clean_quotes(cmds[vars.counter].cmd[vars.helper_i++]);
		vars.helper_i = 0;
	}
}
