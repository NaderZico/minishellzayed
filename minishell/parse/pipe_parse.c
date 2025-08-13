/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:53:06 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/13 17:20:18 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_str_quotes(char *str)
{
	int	i;
	int	quote_char;

	i = 0;
	quote_char = 0;
	while (str[i])
	{
		update_quote_state(str[i], &quote_char);
		i++;
	}
	if (quote_char)
		return (0);
	return (1);
}

static int	check_pipe_pos(char *line)
{
	int	len;
	int	i;

	len = ft_strlen(line);
	if (!len)
		return (1);
	len--;
	while ((line[len] == ' ' || line[len] == '\t') && len > 0)
		len--;
	if (line[len] == '|')
		return (0);
	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] == '|')
		return (0);
	return (1);
}

static int	is_pipe_in_quotes(char *line, int i, int j)
{
	int	quote_char;

	quote_char = 0;
	while (line[i])
	{
		update_quote_state(line[i], &quote_char);
		if (line[i] == '|' && !quote_char)
		{
			j = i + 1;
			while (line[j] == ' ' || line[j] == '\t')
				j++;
			if (line[j] == '\0' || line[j] == '|')
				return (0);
		}
		i++;
	}
	return (1);
}

int	validate_pipes(t_pipe *pipe, char *line, t_cmds *cmds)
{
	int	i;
	int	j;
	int	quotes;

	(void)cmds;
	quotes = 0;
	i = 0;
	j = 0;
	if (!check_pipe_pos(line))
		return (0);
	if (!is_pipe_in_quotes(line, i, j))
		return (0);
	if (!check_str_quotes(line))
		return (0);
	pipe->cmds = ft_split(line, '|');
	i = 0;
	while (pipe->cmds[i])
	{
		expand_dollar_vars(&pipe->cmds[i], pipe);
		handle_spaces(&pipe->cmds[i]);
		i++;
	}
	pipe->total_cmds = i;
	return (1);
}
