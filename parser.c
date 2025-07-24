/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:55:32 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/24 14:17:14 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_error	ensure_command_capacity(t_data *data, int cmd_idx)
{
	int	new_cap;
	int	i;

	if (cmd_idx < data->cmd_cap)
		return (SUCCESS);
	if (data->cmd_cap > 0)
		new_cap = data->cmd_cap * 2;
	else
		new_cap = 16;
	data->commands = ft_realloc(data->commands, data->cmd_cap
			* sizeof * data->commands, new_cap * sizeof * data->commands);
	i = data->cmd_cap;
	while (i < new_cap)
	{
		data->commands[i] = (t_command){NULL, NULL, 0, 0, 0};
		i++;
	}
	data->cmd_cap = new_cap;
	return (SUCCESS);
}

static t_error	add_redirection(t_command *cmd, t_data *data, int *i)
{
	char	*file;

	if (!cmd || !data || !i || !data->tokens)
		return (ERR_SYNTAX);
	if (*i + 1 >= data->token_count || data->tokens[*i + 1].type != WORD)
	{
		print_unexpected_token(data->tokens[*i].type);
		return (ERR_SYNTAX);
	}
	cmd->redirs = ft_realloc(cmd->redirs, cmd->redir_count
			* sizeof * cmd->redirs, (cmd->redir_count + 1)
			* sizeof * cmd->redirs);
	file = ft_strdup(data->tokens[*i + 1].value);
	if (!file)
		return (ERR_MALLOC);
	cmd->redirs[cmd->redir_count++] = (t_redir){file, data->tokens[*i].type};
	*i += 2;
	return (SUCCESS);
}

static t_error	parse_command(t_command *cmd, t_data *data, int *token_i)
{
	t_error	err;

	if (!cmd || !data || !token_i || !data->tokens)
		return (ERR_MALLOC);
	err = SUCCESS;
	*cmd = (t_command){NULL, NULL, 0, 0, 0};
	while (*token_i < data->token_count && data->tokens[*token_i].type != PIPE)
	{
		if (data->tokens[*token_i].type >= REDIR_IN)
			err = add_redirection(cmd, data, token_i);
		else
		{
			cmd->args = ft_extend_arr(cmd->args,
					data->tokens[(*token_i)++].value);
			err = SUCCESS;
		}
		if (err != SUCCESS)
			return (err);
	}
	return (SUCCESS);
}

t_error	parse_tokens(t_data *data)
{
	int		cmd_idx;
	int		token_i;
	t_error	err;

	if (!data || !data->tokens)
		return (ERR_MALLOC);
	cmd_idx = 0;
	token_i = 0;
	free_commands(data);
	while (token_i < data->token_count)
	{
		err = ensure_command_capacity(data, cmd_idx);
		if (err != SUCCESS)
			return (err);
		err = parse_command(&data->commands[cmd_idx], data, &token_i);
		if (err != SUCCESS)
			return (err);
		if (token_i < data->token_count && data->tokens[token_i].type == PIPE)
			token_i++;
		cmd_idx++;
	}
	data->cmd_count = cmd_idx;
	return (SUCCESS);
}
