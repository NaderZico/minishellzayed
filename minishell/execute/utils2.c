/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:43 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 16:59:43 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	get_env(t_pipe *p, char **envp)
{
	int	i;

	i = 0;
	p->env_variables = NULL;
	if (!envp[0])
	{
		write(2, "don't play with me(env -i)\n", 27);
		exit(1);
	}
	while (envp[i])
	{
		ft_lstadd_back(&p->env_variables, ft_lstnew(ft_strdup(envp[i])));
		i++;
	}
	p->env_count = i;
}

void	fill_export(t_pipe *c)
{
	c->m_export = NULL;
	fill_tmp_env(c);
	last_sorting(c);
	c->index = 0;
	while (c->tmp_env[c->index])
	{
		ft_lstadd_back(&c->m_export,
			ft_lstnew(ft_strdup(c->tmp_env[c->index])));
		c->index++;
	}
}

int	env_count(t_pipe *pipe_data)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = pipe_data->env_variables;
	if (pipe_data->env_variables && tmp)
	{
		while (tmp)
		{
			i++;
			if (tmp)
				tmp = tmp->next;
		}
	}
	return (i);
}

void	update_env(t_pipe *c)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = c->env_variables;
	free_strings(c->tmp_env);
	c->env_count = env_count(c);
	c->tmp_env = malloc((c->env_count + 1) * sizeof(char *));
	while (tmp)
	{
		c->tmp_env[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
		i++;
	}
	c->tmp_env[i] = NULL;
	i = 0;
}

void	error_in_exec(t_pipe *c, t_cmds *p)
{
	write(2, p[0].cmd[0], ft_strlen(p[0].cmd[0]));
	if (p[0].cmd[0])
		write(2, ": command not found\n", 21);
	g_exit_code = 127;
	free(c->cmd_exec);
	free_and_exit(c, p);
}
