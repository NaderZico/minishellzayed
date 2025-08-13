/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:46 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 17:00:46 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	custom_strncmp(const char *s1, const char *s2, unsigned int n)
{
	size_t			i;
	unsigned char	*ns1;
	unsigned char	*ns2;

	i = 0;
	if (!s1 || !s2)
		return (1);
	ns1 = (unsigned char *)s1;
	ns2 = (unsigned char *)s2;
	while (i < n && ns1[i] != '\0' && ns2[i] != '\0')
	{
		if (ns1[i] != ns2[i])
			return (ns1[i] - ns2[i]);
		i++;
	}
	if (i != n)
		return (ns1[i] - ns2[i]);
	return (0);
}

static void	reset_values(t_vars *v, t_pipe *pipe_data)
{
	v->cmd_count = 0;
	v->fd = v->i + 1;
	v->redirect_index = env_count(pipe_data);
}

int	found_first(char **m_env, t_pipe *pipe_data)
{
	t_vars	v;

	init_export(&v, pipe_data);
	while (m_env[v.i++])
	{
		reset_values(&v, pipe_data);
		if (m_env[v.i + 1] != NULL && m_env[v.i] != NULL)
		{
			while ((int)m_env[v.i][v.counter] > (int)m_env[v.fd++][v.counter])
			{
				v.cmd_count++;
				if (m_env[v.fd] == NULL)
					break ;
				v.redirect_index--;
			}
			if (m_env[v.fd] != NULL)
			{
				if (!m_env[v.fd + 1])
					return (v.i);
			}
			else
				return (v.i);
		}
	}
	return (0);
}

void	populate_export_list(t_pipe *pipe_data)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = pipe_data->env_variables;
	pipe_data->m_export = NULL;
	while (tmp)
	{
		ft_lstadd_back(&pipe_data->m_export, ft_lstnew(tmp->content));
		tmp = tmp->next;
		i++;
	}
}

void	fill_tmp_env(t_pipe *command_pipe)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = command_pipe->env_variables;
	command_pipe->env_count = env_count(command_pipe);
	command_pipe->tmp_env = malloc(
			(command_pipe->env_count + 1)
			* sizeof(char *));
	while (tmp)
	{
		command_pipe->tmp_env[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	command_pipe->tmp_env[i] = NULL;
}
