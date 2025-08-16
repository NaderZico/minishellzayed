/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:53:23 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/16 13:39:31 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_redir(char **str, t_vars *v)
{
	if ((*str)[v->i] == '>' || (*str)[v->i] == '<')
	{
		v->i++;
		if (((*str)[v->i] == '<' && (*str)[v->i - 1] == '<')
			|| ((*str)[v->i] == '>' && (*str)[v->i - 1] == '>'))
			v->i++;
		while ((*str)[v->i] == ' ' || (*str)[v->i] == '\t')
			v->i++;
		if (((*str)[v->i] == '>' || (*str)[v->i] == '<') && (!v->in_quotes))
			return (0);
		while ((*str)[v->i] == ' ' || (*str)[v->i] == '\t')
			v->i++;
		if ((*str)[v->i] == '\0' || ((*str)[v->i] == '|' && !v->in_quotes))
			return (0);
	}
	else
		v->i++;
	return (1);
}

static int	check_redir_end(char *s)
{
	int	i;

	i = ft_strlen(s);
	if (i)
		i--;
	while ((s[i] == ' ' || s[i] == '\t') && i > 0)
		i--;
	if (s[i] == '>' || s[i] == '<')
		return (0);
	return (1);
}

int	validate_redirs(char *str)
{
	t_vars	vars;

	vars.i = 0;
	vars.cmd_i = 0;
	vars.in_d_quotes = 0;
	vars.in_quotes = 0;
	if (!check_redir_end(str))
		return (0);
	while (str[vars.i])
	{
		update_quote_state(str[vars.i], &vars.in_quotes);
		if (!is_valid_redir(&str, &vars))
			return (0);
	}
	return (1);
}
