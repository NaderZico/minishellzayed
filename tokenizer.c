/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:12:33 by nakhalil          #+#             */
/*   Updated: 2025/07/24 12:20:47 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Handles unquoted words in the input.
 * Extracts the word and adds it as a token.
 */
static t_error	handle_word(char *input, int *i, t_data *data)
{
	int		start;
	t_quote	quote;
	char	*value;

	start = *i;
	quote = NO_QUOTE;
	while (input[*i])
	{
		if (quote == NO_QUOTE && (ft_isspace(input[*i]) || ft_strchr("|<>",
					input[*i])))
			break ;
		if ((input[*i] == '\'' || input[*i] == '"') && quote == NO_QUOTE)
			quote = update_quote_state(quote, input[*i]);
		else if (input[*i] == quote_type_to_char(quote))
			quote = update_quote_state(quote, input[*i]);
		(*i)++;
	}
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (ERR_MALLOC);
	return (add_token(data, value, WORD, NO_QUOTE));
}

/*
 * Handles redirection operators (<, >, <<, >>) in the input.
 * Adds the appropriate redirection token.
 */
static t_error	handle_redirection(char *input, int *i, t_data *data)
{
	t_token_type	type;

	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		type = REDIR_HEREDOC;
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		type = REDIR_IN;
		(*i) += 1;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		type = REDIR_APPEND;
		(*i) += 2;
	}
	else if (input[*i] == '>')
	{
		type = REDIR_OUT;
		(*i) += 1;
	}
	else
		return (ERR_SYNTAX);
	return (add_token(data, NULL, type, NO_QUOTE));
}

/*
 * Handles quoted substrings in the input.
 * Extracts the quoted value and adds it as a token.
 * Returns an error if quotes are not terminated.
 */
static t_error	handle_quotes(char *input, int *i, t_data *data)
{
	t_quote	quote_type;
	int		start;
	char	*value;

	if (input[*i] == '\'')
		quote_type = SINGLE_QUOTE;
	else
		quote_type = DOUBLE_QUOTE;
	start = *i + 1;
	(*i)++;
	while (input[*i] && input[*i] != quote_type_to_char(quote_type))
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd("minishell: unterminated quote\n", 2);
		data->syntax_error = 1;
		return (ERR_SYNTAX);
	}
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (ERR_MALLOC);
	(*i)++;
	return (add_token(data, value, WORD, quote_type));
}

/*
 * Split input string on unquoted delimiter (e.g., '\n' or ';').
 * Returns a NULL-terminated array of strings.
 */
char **split_unquoted(const char *input, char delim)
{
	int i = 0, start = 0, count = 0, len = ft_strlen(input);
	t_quote quote = NO_QUOTE;
	char **result = NULL;

	// First pass: count splits
	while (i < len)
	{
		if ((input[i] == '\'' || input[i] == '"'))
			quote = update_quote_state(quote, input[i]);
		if (input[i] == delim && quote == NO_QUOTE)
			count++;
		i++;
	}
	result = malloc(sizeof(char *) * (count + 2));
	if (!result)
		return NULL;

	// Second pass: extract substrings
	i = 0; int idx = 0; quote = NO_QUOTE;
	while (i <= len)
	{
		if ((input[i] == '\'' || input[i] == '"'))
			quote = update_quote_state(quote, input[i]);
		if ((input[i] == delim && quote == NO_QUOTE) || input[i] == '\0')
		{
			result[idx++] = ft_substr(input, start, i - start);
			start = i + 1;
		}
		i++;
	}
	result[idx] = NULL;
	return result;
}

/*
 * Tokenizes the input string into tokens for the shell.
 * Iterates through the input and delegates to handlers for each token type.
 */
t_error	tokenize_input(char *input, t_data *data)
{
	int		i;
	t_error	err;

	i = 0;
	err = SUCCESS;
	free_tokens(data);
	while (input[i] && err == SUCCESS)
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '\'' || input[i] == '"')
			err = handle_quotes(input, &i, data);
		else if (input[i] == '|')
		{
			i++;
			err = add_token(data, NULL, PIPE, NO_QUOTE);
		}
		else if (input[i] == '<' || input[i] == '>')
			err = handle_redirection(input, &i, data);
		else
			err = handle_word(input, &i, data);
	}
	if (err == SUCCESS && data->syntax_error)
		err = ERR_SYNTAX;
	return (err);
}
