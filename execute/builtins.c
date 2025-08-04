/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 20:27:44 by abdamoha          #+#    #+#             */
/*   Updated: 2025/08/04 12:19:25 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(t_cmds *p, int x, int pm, t_pipe *c)
{
	int	y;

	y = 1;
	(void)pm;
	g_exit_code = 0;
	if (p->redir_count > 0 && pm != 1)
		if (check_exec_redirect(p, c, 0, x) < 0)
			return ;
	if (!p[x].cmd[y] || !p[x].cmd[y][0])
		printf("\n");
	else if (check_for_flag(p[x].cmd[y]) && !p[x].cmd[y - 1][4])
		echo_new_line(p, x, y, c);
	else if (check_for_flag(p[x].cmd[y]) == 0)
		echo_flag(p, x, y, c);
}

void	ft_pwd(t_cmds *p, t_pipe *c, int pm)
{
	char	*pwd;

	(void)p;
	pwd = NULL;
	if (p->redir_count > 0 && pm != 1 && c->current_result != 1)
		check_exec_redirect(p, c, 0, 0);
	pwd = getcwd(NULL, 1024);
	if (pwd != NULL)
		printf("%s\n", pwd);
	else
		g_exit_code = 1;
	free(pwd);
	g_exit_code = 0;
}

void	ft_env(t_cmds *p, t_pipe *c, int pm)
{
	t_list	*tmp;

	(void)p;
	(void)c;
	tmp = c->env_variables;
	if (p->redir_count > 0 && pm != 1 && c->current_result != 1)
		check_exec_redirect(p, c, 0, 0);
	while (tmp)
	{
		if (tmp)
			printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	g_exit_code = 0;
}

void	ft_cd(t_cmds *p, int x, int pm, t_pipe *c)
{
	int	y;

	y = 1;
	if (p->redir_count > 0 && pm != 1 && c->current_result != 1)
		check_exec_redirect(p, c, 0, 0);
	update_pwd(c, getcwd(NULL, 1024), "OLDPWD", 1);
	update_export(c, getcwd(NULL, 1024), "OLDPWD", 1);
	if (chdir(p[x].cmd[y]) < 0)
	{
		perror("cd");
		g_exit_code = 1;
	}
	else
		g_exit_code = 0;
	update_pwd(c, getcwd(NULL, 1024), "PWD", 0);
	update_export(c, getcwd(NULL, 1024), "PWD", 0);
}

void	ft_export(t_pipe *c, t_cmds *p, int i, int fd)
{
	(void)fd;
	c->tmp = NULL;
	c->tmp1 = c->m_export;
	if (p->redir_count > 0 && c->current_result != 1)
		check_exec_redirect(p, c, 0, 0);
	c->loop_counter = 1;
	if (p[i].cmd[c->loop_counter])
		insert_the_node(p, c);
	else
	{
		if (c->tmp1)
		{
			while (c->tmp1)
			{
				if (c->tmp1)
					printf("declare -x %s\n", (char *)c->tmp1->content);
				c->tmp1 = c->tmp1->next;
			}
		}
		g_exit_code = 0;
	}
}
