/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:33 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/01 17:16:31 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_exec(t_cmds *p, t_vars *v, int i, t_pipe *c)
{
	v->current_quote = ft_strjoin(p[i].outs[v->redir_i].file_name, "\n");
	if (ft_strcmp_heredoc(v->line, v->current_quote) == 0)
	{
		break_condition(p, i, v);
		return (1);
	}
	ft_putstr_fd(v->line, v->tmp, 0);
	write(1, "> ", 2);
	free(v->line);
	v->line = get_next_line(0);
	dollar_expansion(&v->line, c);
	free(v->current_quote);
	if (!v->line)
		return (free(v->temp_buffer), 1);
	return (0);
}

char	*create_file(t_pipe *c)
{
	char	*s;

	s = ft_itoa(c->debug_timing_flag);
	c->debug_timing_flag++;
	return (s);
}
