/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 20:14:04 by abdamoha          #+#    #+#             */
/*   Updated: 2025/07/30 14:25:02 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_to_export(t_cmds *p, t_pipe *c, int i, int j)
{
	c->tmp2 = NULL;
	c->tmp1 = c->m_export;
	c->tmp3 = c->tmp1;
	if (check_if_exist(p, i, j, c) == 0 && check_for_equal(p, i, j) == 0)
		return ;
	else if (check_if_exist(p, i, j, c) == 0 && check_for_equal(p, i, j) == 1)
		return (changing_the_value(p, i, j, c), changing_the_env_v(p, i, j, c));
	if (custom_strncmp(p[0].cmd[j], "=", 1) != 0)
	{
		while (c->tmp1)
		{
			if (custom_strncmp(c->tmp1->content, p[0].cmd[j],
					ft_strlen(c->tmp1->content)) > 0)
			{
				export_add(c, i, j, p);
				break ;
			}
			c->tmp3 = c->tmp1;
			c->tmp1 = c->tmp1->next;
			i++;
		}
		if (!c->tmp1)
			export_last(c, p, j);
	}
}

void	insert_the_node(t_cmds *p, t_pipe *c)
{
	int		i;
	int		j;

	i = 0;
	j = 1;
	while (p[i].cmd[j] != NULL)
	{
		if (ft_strcmp(p[i].cmd[j], "=") != 0
			&& ft_isexportable(p[i].cmd[j], len_till_equal(p[i].cmd[j])) == 0)
			add_to_export(p, c, i, j);
		else if (ft_isexportable(p[i].cmd[j],
				len_till_equal(p[i].cmd[j])) == 1
			|| ft_strcmp(p[i].cmd[j], "=") == 0)
		{
			g_exit_code = 1;
			write (2, p[i].cmd[j], ft_strlen(p[i].cmd[j]));
			write(2, ": not a valid identifier\n", 25);
		}
		j++;
	}
}

int	check_if_exist(t_cmds *p, int i, int j, t_pipe *c)
{
	int		len;
	t_list	*t_e;

	len = len_till_equal(p[i].cmd[j]);
	t_e = c->m_export;
	while (t_e)
	{
		if (custom_strncmp(t_e->content, p[i].cmd[j], len) == 0)
			return (0);
		t_e = t_e->next;
	}
	return (1);
}

int	len_till_equal(char *str)
{
	int		i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

int	check_for_flag(char *str)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '-' || str[i] == 'n')
			j++;
		else
			return (1);
		i++;
	}
	return (0);
}
