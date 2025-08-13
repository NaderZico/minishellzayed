/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:53:53 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/13 17:20:55 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	pad_tokens(char **str, int *i, int *j, char **new_str)
{
	(*new_str)[(*j)++] = ' ';
	if ((*str)[*i] == '<' && (*str)[*i + 1] != '<')
		(*new_str)[(*j)++] = '<';
	else if ((*str)[*i] == '<' && (*str)[*i + 1] == '<')
	{
		(*new_str)[(*j)++] = '<';
		(*new_str)[(*j)++] = '<';
		(*i)++;
	}
	else if ((*str)[*i] == '>' && (*str)[*i + 1] != '>')
		(*new_str)[(*j)++] = '>';
	else if ((*str)[*i] == '>' && (*str)[*i + 1] == '>')
	{
		(*new_str)[(*j)++] = '>';
		(*new_str)[(*j)++] = '>';
		(*i)++;
	}
	else if ((*str)[*i] == '\t')
		(*new_str)[(*j)++] = ' ';
	else
	{
		(*new_str)[(*j) - 1] = (*str)[(*i)];
		return ;
	}
	(*new_str)[(*j)++] = ' ';
}

static char	*add_space_tokens(char *str)
{
	char	*new_str;
	int		i;
	int		j;
	int		quote_char;

	i = -1;
	j = 0;
	quote_char = 0;
	new_str = malloc(ft_strlen(str) * 3 + 2);
	while (str[++i])
	{
		update_quote_state(str[i], &quote_char);
		if (!quote_char)
			pad_tokens(&str, &i, &j, &new_str);
		else
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

static void	remove_extra_spaces(char *str)
{
	t_vars	vars;

	vars.i = 0;
	vars.counter = 0;
	vars.space_found = 0;
	vars.quote_char = 0;
	while (str[vars.i] != '\0')
	{
		update_quote_state(str[vars.i], &vars.quote_char);
		if (str[vars.i] == '"' || str[vars.i] == '\'')
		{
			vars.space_found = 0;
			str[vars.counter++] = str[vars.i];
		}
		else if (vars.quote_char == 0)
			increment(str, &vars);
		else
			str[vars.counter++] = str[vars.i];
		vars.i++;
	}
	str[vars.counter] = '\0';
}

void	handle_spaces(char **str)
{
	*str = add_space_tokens(*str);
	remove_extra_spaces(*str);
}
