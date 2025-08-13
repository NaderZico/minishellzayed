/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pipes_exec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:01:41 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:52:04 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_echo(t_cmds *command, int index, int pipe_mode,
		t_pipe *pipe)
{
	int	y;

	y = 1;
	g_exit_code = 0;
	(void)pipe_mode;
	if ((!command[index].cmd[y] || !command[index].cmd[y][0]))
		write(1, "\n", 1);
	else if (check_for_flag(command[index].cmd[y]) && !command[index].cmd[y
			- 1][4])
		echo_new_line(command, index, y, pipe);
	else if (check_for_flag(command[index].cmd[y]) == 0)
		echo_flag(command, index, y, pipe);
}

void	execute_print_working_directory(t_cmds *cmds,
		t_pipe *pipe, int pipe_mode)
{
	char	*pwd;

	(void)cmds;
	(void)pipe;
	(void)pipe_mode;
	pwd = NULL;
	pwd = getcwd(NULL, 1024);
	if (pwd != NULL)
		printf("%s\n", pwd);
	else
		g_exit_code = 1;
	free(pwd);
	g_exit_code = 0;
}

void	execute_env_command(t_cmds *cmds, t_pipe *pipe, int pipe_mode)
{
	t_list	*tmp;

	(void)cmds;
	(void)pipe_mode;
	tmp = pipe->env_variables;
	while (tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	if (pipe->fd1 > 2)
		close(pipe->fd1);
	g_exit_code = 0;
}

void	execute_cd_with_pipes(t_cmds *cmds, int x, int pipe_mode, t_pipe *pipe)
{
	int	y;

	(void)pipe_mode;
	y = 1;
	update_pwd(pipe, getcwd(NULL, 1024), "OLDPWD", 1);
	update_export(pipe, getcwd(NULL, 1024), "OLDPWD", 1);
	if (chdir(cmds[x].cmd[y]) < 0)
	{
		printf("%s: No such file or directory\n", cmds[x].cmd[y]);
		g_exit_code = 1;
	}
	else
		g_exit_code = 0;
	update_pwd(pipe, getcwd(NULL, 1024), "PWD", 0);
	update_export(pipe, getcwd(NULL, 1024), "PWD", 0);
}

void	export_pipe_commands(t_pipe *pipe, t_cmds *cmds, int i, int fd)
{
	(void)fd;
	pipe->tmp = NULL;
	pipe->tmp1 = pipe->m_export;
	pipe->loop_counter = 1;
	if (cmds[i].cmd[pipe->loop_counter])
		insert_the_node(cmds, pipe);
	else
	{
		while (pipe->tmp1)
		{
			printf("declare -x %s\n", (char *)pipe->tmp1->content);
			pipe->tmp1 = pipe->tmp1->next;
		}
		g_exit_code = 0;
	}
}
