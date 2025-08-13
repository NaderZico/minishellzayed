/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:01:28 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 07:52:30 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(t_cmds *cmds, int x, int pipe_mode, t_pipe *pipe)
{
	int	y;

	y = 1;
	(void)pipe_mode;
	g_exit_code = 0;
	if (cmds->red_len > 0 && pipe_mode != 1)
		if (check_exec_redirect(cmds, pipe, 0, x) < 0)
			return ;
	if (!cmds[x].cmd[y] || !cmds[x].cmd[y][0])
		printf("\n");
	else if (check_for_flag(cmds[x].cmd[y]) && !cmds[x].cmd[y - 1][4])
		echo_new_line(cmds, x, y, pipe);
	else if (check_for_flag(cmds[x].cmd[y]) == 0)
		echo_flag(cmds, x, y, pipe);
}

void	ft_pwd(t_cmds *cmds, t_pipe *pipe, int pipe_mode)
{
	char	*pwd;

	(void)cmds;
	pwd = NULL;
	if (cmds->red_len > 0 && pipe_mode != 1 && pipe->current_result != 1)
		check_exec_redirect(cmds, pipe, 0, 0);
	pwd = getcwd(NULL, 1024);
	if (pwd != NULL)
		printf("%s\n", pwd);
	else
		g_exit_code = 1;
	free(pwd);
	g_exit_code = 0;
}

void	ft_env(t_cmds *cmds, t_pipe *pipe, int pipe_mode)
{
	t_list	*tmp;

	(void)cmds;
	(void)pipe;
	tmp = pipe->env_variables;
	if (cmds->red_len > 0 && pipe_mode != 1 && pipe->current_result != 1)
		check_exec_redirect(cmds, pipe, 0, 0);
	while (tmp)
	{
		if (tmp)
			printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	g_exit_code = 0;
}

void	ft_cd(t_cmds *cmds, int x, int pipe_mode, t_pipe *pipe)
{
	int	y;

	y = 1;
	if (cmds->red_len > 0 && pipe_mode != 1 && pipe->current_result != 1)
		check_exec_redirect(cmds, pipe, 0, 0);
	update_pwd(pipe, getcwd(NULL, 1024), "OLDPWD", 1);
	update_export(pipe, getcwd(NULL, 1024), "OLDPWD", 1);
	if (chdir(cmds[x].cmd[y]) < 0)
	{
		perror("cd");
		g_exit_code = 1;
	}
	else
		g_exit_code = 0;
	update_pwd(pipe, getcwd(NULL, 1024), "PWD", 0);
	update_export(pipe, getcwd(NULL, 1024), "PWD", 0);
}

void	ft_export(t_pipe *pipe, t_cmds *cmds, int i, int fd)
{
	(void)fd;
	pipe->tmp = NULL;
	pipe->tmp1 = pipe->m_export;
	if (cmds->red_len > 0 && pipe->current_result != 1)
		check_exec_redirect(cmds, pipe, 0, 0);
	pipe->loop_counter = 1;
	if (cmds[i].cmd[pipe->loop_counter])
		insert_the_node(cmds, pipe);
	else
	{
		if (pipe->tmp1)
		{
			while (pipe->tmp1)
			{
				if (pipe->tmp1)
					printf("declare -x %s\n", (char *)pipe->tmp1->content);
				pipe->tmp1 = pipe->tmp1->next;
			}
		}
		g_exit_code = 0;
	}
}
