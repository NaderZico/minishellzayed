/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:59:35 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/01 17:11:17 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_redir(char **str, t_vars *v)
{
	if ((*str)[v->i] == '>' || (*str)[v->i] == '<')
	{
		v->i++;
		if (((*str)[v->i] == '<' && (*str)[v->i - 1] == '<')
			|| ((*str)[v->i] == '>' && (*str)[v->i
				- 1] == '>'))
			v->i++;
		while ((*str)[v->i] == ' ' || (*str)[v->i] == '\t')
			v->i++;
		if (((*str)[v->i] == '>' || (*str)[v->i] == '<')
			&& (!v->in_quotes))
			return (0);
		while ((*str)[v->i] == ' ' || (*str)[v->i] == '\t')
			v->i++;
		if ((*str)[v->i] == '\0' || ((*str)[v->i] == '|'
				&& !v->in_quotes))
			return (0);
	}
	else
		v->i++;
	return (1);
}

static int	has_trailing_redir(char *s)
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

static int	check_redir_syntax(char **str, t_vars *vars)
{
	if (!has_trailing_redir(*str))
		return (0);
	vars->i = 0;
	while ((*str)[vars->i])
	{
		if ((*str)[vars->i] == '\''
			|| (*str)[vars->i] == '\"')
		{
			if (!vars->in_quotes)
				vars->in_quotes = (*str)[vars->i];
			else if (vars->in_quotes == (*str)[vars->i])
				vars->in_quotes = 0;
		}
		if (!is_redir(str, vars))
			return (0);
	}
	return (1);
}

int	check_redir(char *str)
{
	t_vars	vars;

	vars.i = 0;
	vars.counter = 0;
	vars.in_d_quotes = 0;
	vars.in_quotes = 0;
	if (!check_redir_syntax(&str, &vars))
		return (0);
	return (1);
}
