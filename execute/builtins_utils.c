/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 07:20:21 by abdamoha          #+#    #+#             */
/*   Updated: 2025/07/30 16:20:20 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_executable(t_pipe *c, t_cmds *p)
{
	int	i;

	i = 0;
	if (ft_strchr(p[i].cmd[0], '.') && ft_strchr(p[i].cmd[0], '/'))
	{
		if (ft_strncmp(p[i].cmd[0], "./minishell", 11) == 0)
		{
			change_shlv(p, c, c->env_variables);
			change_shlv(p, c, c->m_export);
			free_all(c, p);
			return (1);
		}
	}
	return (0);
}

static void	shlvl_exec(t_pipe *c, char *s)
{
	s = ft_itoa(increase_shlvl_value(c->tmp1->content));
	c->tmp3 = ft_lstnew(ft_strjoin("SHLVL=", s));
	c->tmp3->next = c->tmp1->next;
	c->tmp2->next = c->tmp3;
	free(c->tmp1->content);
	free(c->tmp1);
}

void	change_shlv(t_cmds *p, t_pipe *c, t_list *lst)
{
	char	*s;

	c->flag_counter = 0;
	s = NULL;
	c->tmp1 = lst;
	c->tmp2 = c->tmp1;
	c->tmp3 = NULL;
	(void)p;
	(void)c;
	while (c->tmp1)
	{
		if (custom_strncmp(c->tmp1->content, "SHLVL=",
				len_till_equal("SHLVL=")) == 0)
		{
			shlvl_exec(c, s);
			break ;
		}
		c->tmp2 = c->tmp1;
		c->tmp1 = c->tmp1->next;
		c->flag_counter++;
	}
	if (s)
		free(s);
}

int	increase_shlvl_value(char *str)
{
	int	i;

	i = len_till_equal(str) + 1;
	return (ft_atoi(str + i) + 1);
}

void	get_path(char **str, t_pipe *c)
{
	char	*s;
	int		i;

	i = 0;
	s = NULL;
	while (str[i])
	{
		if (custom_strncmp("PATH=", str[i], 5) == 0)
		{
			s = str[i] + 5;
			c->m_path = ft_split(s, ':');
			return ;
		}
		i++;
	}
	return ;
}
