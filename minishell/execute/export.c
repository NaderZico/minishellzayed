/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:54 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 07:52:03 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_for_equal(t_cmds *cmds, int i, int j)
{
	int	k;

	k = 0;
	while (cmds[i].cmd[j][k])
	{
		if (cmds[i].cmd[j][k] == '=')
			return (1);
		k++;
	}
	return (0);
}

void	changing_the_value(t_cmds *cmds, int i, int j, t_pipe *pipe)
{
	pipe->flag_counter = 0;
	pipe->tmp1 = pipe->m_export;
	pipe->tmp2 = pipe->tmp1;
	pipe->tmp3 = NULL;
	while (pipe->tmp1)
	{
		if (custom_strncmp(pipe->tmp1->content, cmds[i].cmd[j],
				len_till_equal(cmds[i].cmd[j])) == 0)
		{
			pipe->tmp3 = ft_lstnew(ft_strdup(cmds[i].cmd[j]));
			pipe->tmp3->next = pipe->tmp1->next;
			if (pipe->tmp2->content != pipe->tmp1->content)
				pipe->tmp2->next = pipe->tmp3;
			break ;
		}
		pipe->tmp2 = pipe->tmp1;
		pipe->tmp1 = pipe->tmp1->next;
		pipe->flag_counter++;
	}
	if (pipe->tmp2->content != pipe->tmp1->content)
		pipe->tmp2->next = pipe->tmp3;
	free(pipe->tmp1->content);
	free(pipe->tmp1);
	if (pipe->flag_counter == 0)
		pipe->m_export = pipe->tmp3;
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

void	changing_the_env_v(t_cmds *cmds, int i, int j, t_pipe *pipe)
{
	pipe->flag_counter = 0;
	pipe->tmp1 = pipe->env_variables;
	pipe->tmp2 = pipe->tmp1;
	pipe->tmp3 = NULL;
	while (pipe->tmp1)
	{
		if (custom_strncmp(pipe->tmp1->content, cmds[i].cmd[j],
				len_till_equal(cmds[i].cmd[j])) == 0)
		{
			pipe->tmp3 = ft_lstnew(ft_strdup(cmds[i].cmd[j]));
			pipe->tmp3->next = pipe->tmp1->next;
			if (pipe->tmp2->content != pipe->tmp1->content)
				pipe->tmp2->next = pipe->tmp3;
			break ;
		}
		pipe->tmp2 = pipe->tmp1;
		pipe->tmp1 = pipe->tmp1->next;
		pipe->flag_counter++;
	}
	if (pipe->tmp2->content != pipe->tmp1->content)
		pipe->tmp2->next = pipe->tmp3;
	free(pipe->tmp1->content);
	free(pipe->tmp1);
	if (pipe->flag_counter == 0)
		pipe->env_variables = pipe->tmp3;
}

void	unset_cmp(t_list **lst, char *str, t_pipe *pipe)
{
	pipe->tmp1 = (*lst);
	pipe->tmp2 = pipe->tmp1;
	while (pipe->tmp1)
	{
		if (custom_strncmp(pipe->tmp1->content, str, len_till_equal(str)) == 0)
		{
			if (pipe->tmp2->content == pipe->tmp1->content)
				(*lst) = (*lst)->next;
			else if (pipe->tmp1->next)
			{
				pipe->tmp3 = pipe->tmp1->next;
				pipe->tmp2->next = pipe->tmp3;
			}
			else
				pipe->tmp2->next = NULL;
			free(pipe->tmp1->content);
			free(pipe->tmp1);
			return ;
		}
		pipe->tmp2 = pipe->tmp1;
		pipe->tmp1 = pipe->tmp1->next;
	}
}
