/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pipes_exec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 21:23:49 by abdamoha          #+#    #+#             */
/*   Updated: 2025/07/30 16:26:34 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_echo(t_cmds *command, int index, int pipeMode,
		t_pipe *pipeStructure)
{
	int	y;

	y = 1;
	g_exit_code = 0;
	(void)pipeMode;
	if ((!command[index].cmd[y] || !command[index].cmd[y][0]))
		write(1, "\n", 1);
	else if (check_for_flag(command[index].cmd[y]) && !command[index].cmd[y
			- 1][4])
		echo_new_line(command, index, y, pipeStructure);
	else if (check_for_flag(command[index].cmd[y]) == 0)
		echo_flag(command, index, y, pipeStructure);
}

void	execute_print_working_directory(t_cmds *p, t_pipe *c, int pm)
{
	char	*pwd;

	(void)p;
	(void)c;
	(void)pm;
	pwd = NULL;
	pwd = getcwd(NULL, 1024);
	if (pwd != NULL)
		printf("%s\n", pwd);
	else
		g_exit_code = 1;
	free(pwd);
	g_exit_code = 0;
}

void	execute_env_command(t_cmds *p, t_pipe *c, int pm)
{
	t_list	*tmp;

	(void)p;
	(void)pm;
	tmp = c->env_variables;
	while (tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	if (c->fd1 > 2)
		close(c->fd1);
	g_exit_code = 0;
}

void	execute_cd_with_pipes(t_cmds *p, int x, int pm, t_pipe *c)
{
	int	y;

	(void)pm;
	y = 1;
	update_pwd(c, getcwd(NULL, 1024), "OLDPWD", 1);
	update_export(c, getcwd(NULL, 1024), "OLDPWD", 1);
	if (chdir(p[x].cmd[y]) < 0)
	{
		printf("%s: No such file or directory\n", p[x].cmd[y]);
		g_exit_code = 1;
	}
	else
		g_exit_code = 0;
	update_pwd(c, getcwd(NULL, 1024), "PWD", 0);
	update_export(c, getcwd(NULL, 1024), "PWD", 0);
}

void	export_pipe_commands(t_pipe *c, t_cmds *p, int i, int fd)
{
	(void)fd;
	c->tmp = NULL;
	c->tmp1 = c->m_export;
	c->loop_counter = 1;
	if (p[i].cmd[c->loop_counter])
		insert_the_node(p, c);
	else
	{
		while (c->tmp1)
		{
			printf("declare -x %s\n", (char *)c->tmp1->content);
			c->tmp1 = c->tmp1->next;
		}
		g_exit_code = 0;
	}
}
