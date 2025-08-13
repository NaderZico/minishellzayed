/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:33 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/13 13:29:41 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_exec(t_cmds *cmds, t_vars *v, int i, t_pipe *pipe)
{
	v->current_quote = ft_strjoin(cmds[i].outs[v->redirect_index].file_name,
			"\n");
	if (ft_strcmp_heredoc(v->line, v->current_quote) == 0)
	{
		break_condition(cmds, i, v);
		return (1);
	}
	ft_putstr_fd(v->line, v->tmp, 0);
	write(1, "> ", 2);
	free(v->line);
	v->line = get_next_line(0);
	expand_dollar_vars(&v->line, pipe);
	free(v->current_quote);
	if (!v->line)
		return (free(v->temp_buffer), 1);
	return (0);
}

char	*create_file(t_pipe *pipe)
{
	char	*s;

	s = ft_itoa(pipe->debug_timing_flag);
	pipe->debug_timing_flag++;
	return (s);
}
