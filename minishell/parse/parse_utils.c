/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:53:15 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/13 17:52:40 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_redirs(char *str)
{
	int	i;
	int	num;
	int	in_quote;

	i = 0;
	num = 0;
	in_quote = 0;
	while (str[i])
	{
		update_quote_state(str[i], &in_quote);
		if (!in_quote && (str[i] == '>' || str[i] == '<'))
		{
			if (str[i + 1] == '>' || str[i + 1] == '<')
				i++;
			num++;
		}
		i++;
	}
	return (num);
}



void	update_quote_state(char c, int *quote_char)
{
	if ((c == '"' || c == '\''))
	{
		if (*quote_char == 0)
			*quote_char = c;
		else if (*quote_char == c)
			*quote_char = 0;
	}
}

void	clean_quotes(char *str)
{
	int	i;
	int	j;
	int	quote_char;

	j = 0;
	i = 0;
	quote_char = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (!quote_char || quote_char == str[i])
				update_quote_state(str[i], &quote_char);
			else
				str[j++] = str[i];
		}
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}
