/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:03:01 by nakhalil          #+#    #+#             */
/*   Updated: 2025/05/24 18:38:36 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_unexpected_token(t_token_type type)
{
	if (type == PIPE)
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `|'\n", 2);
	else if (type == REDIR_IN)
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `<'\n", 2);
	else if (type == REDIR_OUT)
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `>'\n", 2);
	else if (type == REDIR_HEREDOC)
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `<<'\n", 2);
	else if (type == REDIR_APPEND)
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `>>'\n", 2);
}

t_error	check_pipe_sequence(t_data *data, int *i, int *prev_was_word)
{
	while (*i < data->token_count)
	{
		if (data->tokens[*i].type == PIPE)
		{
			if (!(*prev_was_word) || (*i == data->token_count - 1))
			{
				print_unexpected_token(PIPE);
				return (ERR_SYNTAX);
			}
			*prev_was_word = 0;
		}
		else
			*prev_was_word
				= (data->tokens[*i].type == WORD);
		(*i)++;
	}
	return (SUCCESS);
}

t_error	check_redir_sequence(t_data *data, int *i)
{
	while (*i < data->token_count)
	{
		if (data->tokens[*i].type >= REDIR_IN
			&& data->tokens[*i].type <= REDIR_HEREDOC)
		{
			if (*i + 1 >= data->token_count
				|| data->tokens[*i + 1].type != WORD)
			{
				print_unexpected_token(data->tokens[*i].type);
				return (ERR_SYNTAX);
			}
			if (*i > 0
				&& data->tokens[*i - 1].type >= REDIR_IN
				&& data->tokens[*i - 1].type
					<= REDIR_HEREDOC)
			{
				print_unexpected_token(data->tokens[*i].type);
				return (ERR_SYNTAX);
			}
			*i += 2;
		}
		else
			(*i)++;
	}
	return (SUCCESS);
}
