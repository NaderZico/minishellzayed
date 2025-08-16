/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:53:23 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/16 17:27:38 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	validate_redir_syntax(char *str, t_vars *vars)
{
	vars->i++;
	if ((str[vars->i] == '<' && str[vars->i - 1] == '<') || (str[vars->i] == '>'
			&& str[vars->i - 1] == '>'))
		vars->i++;
	while (str[vars->i] == ' ' || str[vars->i] == '\t')
		vars->i++;
	if ((str[vars->i] == '>' || str[vars->i] == '<') && !vars->in_quotes)
		return (0);
	while (str[vars->i] == ' ' || str[vars->i] == '\t')
		vars->i++;
	if (str[vars->i] == '\0' || (str[vars->i] == '|' && !vars->in_quotes))
		return (0);
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
		if ((str[vars.i] == '>' || str[vars.i] == '<') && !vars.in_quotes)
		{
			if (!validate_redir_syntax(str, &vars))
				return (0);
			continue ;
		}
		vars.i++;
	}
	return (1);
}
