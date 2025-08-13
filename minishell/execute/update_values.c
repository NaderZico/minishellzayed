/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_values.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:49 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:57:39 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd(t_pipe *pipe, char *new_path, char *var_name, int is_oldpwd)
{
	pipe->tmp1 = pipe->env_variables;
	while (pipe->tmp1)
	{
		if (custom_strncmp(var_name, pipe->tmp1->content,
				len_till_equal(pipe->tmp1->content)) == 0)
		{
			free(pipe->tmp1->content);
			if (is_oldpwd == 0)
				pipe->tmp1->content = ft_strjoin("PWD=", new_path);
			else
				pipe->tmp1->content = ft_strjoin("OLDPWD=", new_path);
			break ;
		}
		pipe->tmp1 = pipe->tmp1->next;
	}
	free(new_path);
}

void	update_export(t_pipe *pipe, char *new_path,
	char *var_name, int is_oldpwd)
{
	pipe->tmp1 = pipe->m_export;
	while (pipe->tmp1)
	{
		if (custom_strncmp(var_name, pipe->tmp1->content,
				len_till_equal(pipe->tmp1->content)) == 0)
		{
			free(pipe->tmp1->content);
			if (is_oldpwd == 0)
				pipe->tmp1->content = ft_strjoin("PWD=", new_path);
			else
				pipe->tmp1->content = ft_strjoin("OLDPWD=", new_path);
			break ;
		}
		pipe->tmp1 = pipe->tmp1->next;
	}
	free(new_path);
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

void	init_export(t_vars *v, t_pipe *var_name)
{
	v->i = 0;
	v->fd = 0;
	v->cmd_count = 0;
	v->counter = 0;
	v->redirect_index = env_count(var_name);
}

void	wait_pipes(t_vars *v, t_pipe *pipe, t_cmds *var_name)
{
	v->helper_index = 0;
	while (v->helper_index < var_name->cmd_len)
	{
		wait(&pipe->status);
		if (v->helper_index == var_name->cmd_len - 1)
			exit_code_pipes(pipe, v);
		v->helper_index++;
	}
	pipe->current_result = 0;
	pipe->pipe_flag_read = 0;
	pipe->pipe_flag_read_index = 0;
}
