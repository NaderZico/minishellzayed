/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:23:54 by nakhalil          #+#    #+#             */
/*   Updated: 2025/07/24 11:34:49 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_data *data)
{
	int	i;

	if (!data || !data->tokens)
	{
		if (data)
		{
			data->tokens     = NULL;
			data->token_count = 0;
			data->token_cap   = 0;
		}
		return;
	}
	i = 0;
	while (i < data->token_count)
	{
		free(data->tokens[i].value);
		i++;
	}
	free(data->tokens);
	data->tokens     = NULL;
	data->token_count = 0;
	data->token_cap   = 0;
}

void	free_commands(t_data *data)
{
	int	c;
	int	r;

	if (!data || !data->commands)
	{
		if (data)
		{
			data->commands = NULL;
			data->cmd_count = 0;
			data->cmd_cap   = 0;
		}
		return;
	}
	c = 0;
	while (c < data->cmd_count)
	{
		ft_free_arr(data->commands[c].args);
		r = 0;
		while (r < data->commands[c].redir_count)
		{
			free(data->commands[c].redirs[r].file);
			r++;
		}
		free(data->commands[c].redirs);
		c++;
	}
	free(data->commands);
	data->commands = NULL;
	data->cmd_count = 0;
	data->cmd_cap   = 0;
}

void	free_data(t_data *data)
{
	if (!data)
		return;
	free_tokens(data);
	free_commands(data);
}

void	ft_free_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
