/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:54:24 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/24 11:34:49 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	quote_type_to_char(t_quote type)
{
	if (type == SINGLE_QUOTE)
		return ('\'');
	if (type == DOUBLE_QUOTE)
		return ('"');
	return (0);
}

/*
 * Updates the quote state based on the current character.
 * Toggles between quoted and unquoted states.
 */
t_quote	update_quote_state(t_quote current, char c)
{
	if (c == '\'')
	{
		if (current == SINGLE_QUOTE)
			return (NO_QUOTE);
		else
			return (SINGLE_QUOTE);
	}
	if (c == '"')
	{
		if (current == DOUBLE_QUOTE)
			return (NO_QUOTE);
		else
			return (DOUBLE_QUOTE);
	}
	return (current);
}

/*
 * Ensures there is enough capacity in the token array.
 * Doubles the capacity if needed.
 */
t_error	ensure_token_capacity(t_data *data)
{
	int	new_cap;

	if (!data)
		return (ERR_MALLOC);
	if (data->token_count < data->token_cap)
		return (SUCCESS);
	if (data->token_cap > 0)
		new_cap = data->token_cap * 2;
	else
		new_cap = 16;
	data->tokens = ft_realloc(data->tokens, data->token_cap
			* sizeof *data->tokens, new_cap * sizeof *data->tokens);
	data->token_cap = new_cap;
	return (SUCCESS);
}

/*
 * Adds a token to the token array.
 * Handles allocation and initialization of token values.
 */
t_error	add_token(t_data *data, char *value, t_token_type type, t_quote quote)
{
	if (!data)
		return (ERR_MALLOC);
	if (ensure_token_capacity(data) != SUCCESS)
		return (ERR_MALLOC);
	if (type != WORD && !value)
	{
		if (type == PIPE)
			value = ft_strdup("|");
		else if (type == REDIR_IN)
			value = ft_strdup("<");
		else if (type == REDIR_OUT)
			value = ft_strdup(">");
		else if (type == REDIR_APPEND)
			value = ft_strdup(">>");
		else if (type == REDIR_HEREDOC)
			value = ft_strdup("<<");
		if (!value)
			return (ERR_MALLOC);
	}
	data->tokens[data->token_count++] = (t_token){value, type, quote};
	return (SUCCESS);
}
