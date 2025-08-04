/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_f.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 00:58:21 by abdamoha          #+#    #+#             */
/*   Updated: 2025/08/04 12:19:25 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	before_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (c->pid == 0)
	{
		if (v->counter == 0)
			first_cmd(c, p, v);
		else if (v->counter == p->cmd_len - 1)
			third2_cmd(c, p, v);
		else
		{
			if (v->i % 2 == 1)
				fourth_cmd(c, p, v);
			else
				fifth_cmd(c, p, v);
			sixth_cmd(c, p, v);
		}
	}
}

void	initialize_variables(t_vars *v, t_pipe *c)
{
	v->error_flag_string = ".efd";
	v->i = 0;
	v->counter = 0;
	v->helper_i = 0;
	v->i = 0;
	v->d = 0;
	c->cmd_exec = NULL;
	c->current_result = 1;
	c->e_fd = open(v->error_flag_string, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (c->e_fd < 0)
	{
		printf("error\n");
	}
}

void	fifth_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (p[v->counter].redir_count > 0)
		check_exec_redirect(p, c, 1, v->counter);
	if (input_check(p, c, v->counter) == 0)
	{
		printf("here");
		if (dup2(c->pipe_fds[1][0], STDIN_FILENO) == -1)
		{
			printf("error12\n");
			exit(0);
		}
	}
	if (output_check(p, c, v->counter) == 0)
	{
		if (dup2(c->pipe_fds[0][1], STDOUT_FILENO) == -1)
		{
			printf("error14\n");
			exit(0);
		}
	}
}

void	exit_code_pipes(t_pipe *c, t_vars *v)
{
	char	*s;
	int		fd;

	(void)c;
	s = NULL;
	close(c->e_fd);
	fd = open(v->error_flag_string, O_RDONLY);
	if (fd < 0)
	{
		g_exit_code = 0;
		return ;
	}
	s = get_next_line(fd);
	g_exit_code = 0;
	if (s)
	{
		g_exit_code = ft_atoi(s);
	}
	free(s);
	close(fd);
}

void	child_exit(t_cmds *p, int j, t_pipe *c, int fd)
{
	g_exit_code = 127;
	write(2, p[j].cmd[0], ft_strlen(p[j].cmd[0]));
	write(2, ": command not found\n", 21);
	if (fd > 0)
	{
		ft_putnbr_fd(g_exit_code, fd);
	}
	close(fd);
	free_and_exit(c, p);
}
