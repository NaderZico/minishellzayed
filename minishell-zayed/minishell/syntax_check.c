/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:32:31 by nakhalil          #+#    #+#             */
/*   Updated: 2025/05/24 18:38:38 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_error	check_pipes(t_data *data)
{
	int	i;
	int	prev;

	if (data->token_count == 0)
		return (SUCCESS);
	if (data->tokens[0].type == PIPE
		|| data->tokens[data->token_count - 1].type == PIPE)
	{
		data->error_pos = (data->tokens[0].type == PIPE) ? 0 : data->token_count - 1;
		return (ERR_SYNTAX);
	}
	prev = 1;
	i = 1;
	return (check_pipe_sequence(data, &i, &prev));
}

static t_error	check_redirs(t_data *data)
{
	int	i;

	if (data->token_count == 0)
		return (SUCCESS);
	if (data->tokens[0].type >= REDIR_IN
		&& data->tokens[0].type <= REDIR_HEREDOC)
	{
		data->error_pos = 0;
		return (ERR_SYNTAX);
	}
	i = 0;
	return (check_redir_sequence(data, &i));
}

static t_error	check_delims(t_data *data)
{
	int	i;

	i = 1;
	while (i < data->token_count)
	{
		if (data->tokens[i].type != WORD
			&& data->tokens[i - 1].type != WORD)
		{
			data->error_pos = i;
			return (ERR_SYNTAX);
		}
		i++;
	}
	return (SUCCESS);
}

t_error	validate_syntax(t_data *data)
{
	t_error	err;

	if (data->token_count == 1
		&& data->tokens[0].type == WORD
		&& data->tokens[0].value[0] == '\0')
	{
		data->error_pos = 0;
		return (ERR_CMD_NOT_FOUND);
	}
	err = check_pipes(data);
	if (err != SUCCESS)
		return (err);
	err = check_redirs(data);
	if (err != SUCCESS)
		return (err);
	err = check_delims(data);
	return (err);
}
