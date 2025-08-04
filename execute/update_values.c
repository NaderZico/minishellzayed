/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_values.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 21:12:58 by abdamoha          #+#    #+#             */
/*   Updated: 2025/08/01 17:16:49 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd(t_pipe *c, char *str, char *p, int k)
{
	c->tmp1 = c->env_variables;
	while (c->tmp1)
	{
		if (custom_strncmp(p, c->tmp1->content,
				len_till_equal(c->tmp1->content)) == 0)
		{
			free(c->tmp1->content);
			if (k == 0)
				c->tmp1->content = ft_strjoin("PWD=", str);
			else
				c->tmp1->content = ft_strjoin("OLDPWD=", str);
			break ;
		}
		c->tmp1 = c->tmp1->next;
	}
	free(str);
}

void	update_export(t_pipe *c, char *str, char *p, int k)
{
	c->tmp1 = c->m_export;
	while (c->tmp1)
	{
		if (custom_strncmp(p, c->tmp1->content,
				len_till_equal(c->tmp1->content)) == 0)
		{
			free(c->tmp1->content);
			if (k == 0)
				c->tmp1->content = ft_strjoin("PWD=", str);
			else
				c->tmp1->content = ft_strjoin("OLDPWD=", str);
			break ;
		}
		c->tmp1 = c->tmp1->next;
	}
	free(str);
}

int	ft_strcmp_heredoc(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

void	init_export(t_vars *v, t_pipe *p)
{
	v->i = 0;
	v->fd = 0;
	v->cmd_count = 0;
	v->counter = 0;
	v->redir_i = env_count(p);
}

void	wait_pipes(t_vars *v, t_pipe *c, t_cmds *p)
{
	v->helper_i = 0;
	while (v->helper_i < p->cmd_len)
	{
		wait(&c->status);
		if (v->helper_i == p->cmd_len - 1)
			exit_code_pipes(c, v);
		v->helper_i++;
	}
	c->current_result = 0;
	c->pipe_flag_read = 0;
	c->pipe_flag_read_index = 0;
}
