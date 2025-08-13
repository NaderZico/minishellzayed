/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:53:29 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/13 14:29:14 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(const char *name, t_pipe *pipe)
{
	char	*value;
	size_t	name_len;
	t_list	*env;
	char	*key;

	value = NULL;
	name_len = ft_strlen(name);
	env = pipe->env_variables;
	while (env != NULL)
	{
		key = (char *)env->content;
		if (ft_strncmp(key, name, name_len) == 0 && key[name_len] == '=')
		{
			value = key + name_len + 1;
			break ;
		}
		env = env->next;
	}
	return (value);
}
