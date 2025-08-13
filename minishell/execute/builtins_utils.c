/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:01:35 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 07:52:22 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_executable(t_pipe *pipe, t_cmds *cmds)
{
	int	i;

	i = 0;
	if (ft_strchr(cmds[i].cmd[0], '.') && ft_strchr(cmds[i].cmd[0], '/'))
	{
		if (ft_strncmp(cmds[i].cmd[0], "./minishell", 11) == 0)
		{
			change_shlv(cmds, pipe, pipe->env_variables);
			change_shlv(cmds, pipe, pipe->m_export);
			free_all(pipe, cmds);
			return (1);
		}
	}
	return (0);
}

static void	shlvl_exec(t_pipe *pipe, char *s)
{
	s = ft_itoa(increase_shlvl_value(pipe->tmp1->content));
	pipe->tmp3 = ft_lstnew(ft_strjoin("SHLVL=", s));
	pipe->tmp3->next = pipe->tmp1->next;
	pipe->tmp2->next = pipe->tmp3;
	free(pipe->tmp1->content);
	free(pipe->tmp1);
}

void	change_shlv(t_cmds *cmds, t_pipe *pipe, t_list *lst)
{
	char	*s;

	pipe->flag_counter = 0;
	s = NULL;
	pipe->tmp1 = lst;
	pipe->tmp2 = pipe->tmp1;
	pipe->tmp3 = NULL;
	(void)cmds;
	(void)pipe;
	while (pipe->tmp1)
	{
		if (custom_strncmp(pipe->tmp1->content, "SHLVL=",
				len_till_equal("SHLVL=")) == 0)
		{
			shlvl_exec(pipe, s);
			break ;
		}
		pipe->tmp2 = pipe->tmp1;
		pipe->tmp1 = pipe->tmp1->next;
		pipe->flag_counter++;
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

void	get_path(char **str, t_pipe *pipe)
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
			pipe->m_path = ft_split(s, ':');
			return ;
		}
		i++;
	}
	return ;
}
