/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 18:04:59 by abdamoha          #+#    #+#             */
/*   Updated: 2025/07/30 16:08:08 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_for_equal(t_cmds *p, int i, int j)
{
	int	k;

	k = 0;
	while (p[i].cmd[j][k])
	{
		if (p[i].cmd[j][k] == '=')
			return (1);
		k++;
	}
	return (0);
}

void	changing_the_value(t_cmds *p, int i, int j, t_pipe *c)
{
	c->flag_counter = 0;
	c->tmp1 = c->m_export;
	c->tmp2 = c->tmp1;
	c->tmp3 = NULL;
	while (c->tmp1)
	{
		if (custom_strncmp(c->tmp1->content, p[i].cmd[j],
				len_till_equal(p[i].cmd[j])) == 0)
		{
			c->tmp3 = ft_lstnew(ft_strdup(p[i].cmd[j]));
			c->tmp3->next = c->tmp1->next;
			if (c->tmp2->content != c->tmp1->content)
				c->tmp2->next = c->tmp3;
			break ;
		}
		c->tmp2 = c->tmp1;
		c->tmp1 = c->tmp1->next;
		c->flag_counter++;
	}
	if (c->tmp2->content != c->tmp1->content)
		c->tmp2->next = c->tmp3;
	free(c->tmp1->content);
	free(c->tmp1);
	if (c->flag_counter == 0)
		c->m_export = c->tmp3;
}

char	*env_index(int index, t_list *env)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = env;
	while (i < index && tmp)
	{
		tmp = tmp->next;
		i++;
	}
	if (tmp)
		return (tmp->content);
	else
		return (NULL);
}

void	changing_the_env_v(t_cmds *p, int i, int j, t_pipe *c)
{
	c->flag_counter = 0;
	c->tmp1 = c->env_variables;
	c->tmp2 = c->tmp1;
	c->tmp3 = NULL;
	while (c->tmp1)
	{
		if (custom_strncmp(c->tmp1->content, p[i].cmd[j],
				len_till_equal(p[i].cmd[j])) == 0)
		{
			c->tmp3 = ft_lstnew(ft_strdup(p[i].cmd[j]));
			c->tmp3->next = c->tmp1->next;
			if (c->tmp2->content != c->tmp1->content)
				c->tmp2->next = c->tmp3;
			break ;
		}
		c->tmp2 = c->tmp1;
		c->tmp1 = c->tmp1->next;
		c->flag_counter++;
	}
	if (c->tmp2->content != c->tmp1->content)
		c->tmp2->next = c->tmp3;
	free(c->tmp1->content);
	free(c->tmp1);
	if (c->flag_counter == 0)
		c->env_variables = c->tmp3;
}

void	unset_cmp(t_list **lst, char *str, t_pipe *c)
{
	c->tmp1 = (*lst);
	c->tmp2 = c->tmp1;
	while (c->tmp1)
	{
		if (custom_strncmp(c->tmp1->content, str, len_till_equal(str)) == 0)
		{
			if (c->tmp2->content == c->tmp1->content)
				(*lst) = (*lst)->next;
			else if (c->tmp1->next)
			{
				c->tmp3 = c->tmp1->next;
				c->tmp2->next = c->tmp3;
			}
			else
				c->tmp2->next = NULL;
			free(c->tmp1->content);
			free(c->tmp1);
			return ;
		}
		c->tmp2 = c->tmp1;
		c->tmp1 = c->tmp1->next;
	}
}
