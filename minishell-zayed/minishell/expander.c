/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:25:55 by nakhalil          #+#    #+#             */
/*   Updated: 2025/05/21 20:02:42 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>

char	*get_env_value(char *name, char **env)
{
	size_t	len;
	int		i;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len) && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

t_error	expand_tokens(t_data *data)
{
	size_t	cap;
	size_t	len;
	char	*buf;
	char	*src;
	char	quote;
	size_t	i;
	size_t	start;
	char	*num;
	char	*name;
	char	*val;
	size_t	j;

	i = 0;
	while (i < (size_t)data->token_count)
	{
		if (data->tokens[i].type == WORD
			&& data->tokens[i].quote != SINGLE_QUOTE)
		{
			cap = EXPAND_INIT_CAP;
			len = 0;
			buf = safe_malloc(cap);
			buf[0] = '\0';
			src = data->tokens[i].value;
			quote = 0;
			start = 0;
			j = 0;
			while (src[j])
			{
				if (!quote && (src[j] == '"' || src[j] == '\''))
				{
					quote = src[j++];
					continue;
				}
				if (quote && src[j] == quote)
				{
					quote = 0;
					j++;
					continue;
				}
				if (src[j] == '$' && quote != '\'')
				{
					char next = src[j + 1];
					if (next == '\0'
						|| (next != '?'
							&& !ft_isalnum(next)
							&& next != '_'))
					{
						append_char(&buf, &cap, &len, src[j++]);
						continue;
					}
					j++;
					if (src[j] == '?')
					{
						num = ft_itoa(data->last_status);
						if (!num)
						{
							free(buf);
							return (ERR_MALLOC);
						}
						start = 0;
						while (num[start])
							append_char(&buf, &cap, &len, num[start++]);
						free(num);
						j++;
						continue;
					}
					start = j;
					while (src[j] && (ft_isalnum(src[j]) || src[j] == '_'))
						j++;
					name = ft_substr(src, start, j - start);
					if (!name)
					{
						free(buf);
						return (ERR_MALLOC);
					}
					val = get_env_value(name, data->env);
					free(name);
					start = 0;
					while (val && val[start])
						append_char(&buf, &cap, &len, val[start++]);
					continue;
				}
				append_char(&buf, &cap, &len, src[j++]);
			}
			buf[len] = '\0';
			free(data->tokens[i].value);
			data->tokens[i].value = buf;
		}
		i++;
	}
	return (SUCCESS);
}
