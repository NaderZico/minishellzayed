/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 20:14:04 by abdamoha          #+#    #+#             */
/*   Updated: 2025/08/05 08:56:58 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_to_export(t_cmds *cmd, t_pipe *pipe, int i, int j)
{
	pipe->tmp1 = pipe->m_export;
	pipe->tmp3 = pipe->tmp1;
	if (check_if_exist(cmd, i, j, pipe) == 0 && check_for_equal(cmd, i, j) == 0)
		return ;
	else if (check_if_exist(cmd, i, j, pipe) == 0 && check_for_equal(cmd, i,
			j) == 1)
		return (changing_the_value(cmd, i, j, pipe), changing_the_env_v(cmd, i,
				j, pipe));
	if (custom_strncmp(cmd[0].cmd[j], "=", 1) != 0)
	{
		while (pipe->tmp1)
		{
			if (custom_strncmp(pipe->tmp1->content, cmd[0].cmd[j],
					ft_strlen(pipe->tmp1->content)) > 0)
			{
				export_add(pipe, i, j, cmd);
				break ;
			}
			pipe->tmp3 = pipe->tmp1;
			pipe->tmp1 = pipe->tmp1->next;
			i++;
		}
		if (!pipe->tmp1)
			export_last(pipe, cmd, j);
	}
}

void	insert_the_node(t_cmds *cmd, t_pipe *pipe)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (cmd[i].cmd[j] != NULL)
	{
		if (ft_strcmp(cmd[i].cmd[j], "=") != 0 && ft_isexportable(cmd[i].cmd[j],
				len_till_equal(cmd[i].cmd[j])) == 0)
		{
			pipe->tmp2 = NULL;
			add_to_export(cmd, pipe, i, j);
		}
		else if (ft_isexportable(cmd[i].cmd[j],
				len_till_equal(cmd[i].cmd[j])) == 1 || ft_strcmp(cmd[i].cmd[j],
				"=") == 0)
		{
			g_exit_code = 1;
			write(2, cmd[i].cmd[j], ft_strlen(cmd[i].cmd[j]));
			write(2, ": not a valid identifier\n", 25);
		}
		j++;
	}
}

int	check_if_exist(t_cmds *cmd, int i, int j, t_pipe *pipe)
{
	int		len;
	t_list	*t_e;

	len = len_till_equal(cmd[i].cmd[j]);
	t_e = pipe->m_export;
	while (t_e)
	{
		if (custom_strncmp(t_e->content, cmd[i].cmd[j], len) == 0)
			return (0);
		t_e = t_e->next;
	}
	return (1);
}

int	len_till_equal(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

int	check_for_flag(char *str)
{
	int	i;
	int	j;

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
