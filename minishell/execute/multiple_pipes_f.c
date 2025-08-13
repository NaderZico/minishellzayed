/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_f.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:21 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:12:09 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	before_cmd(t_pipe *pipe, t_cmds *cmd, t_vars *vars)
{
	if (pipe->pid == 0)
	{
		if (vars->counter == 0)
			execute_first_command(pipe, cmd, vars);
		else if (vars->counter == cmd->cmd_len - 1)
			execute_last_command(pipe, cmd, vars);
		else
		{
			if (vars->i % 2 == 1)
				execute_middle_command_odd(pipe, cmd, vars);
			else
				execute_middle_command_even(pipe, cmd, vars);
			finalize_middle_command(pipe, cmd, vars);
		}
	}
}

void	initialize_variables(t_vars *vars, t_pipe *pipe)
{
	vars->temp_exit_file = ".minishell_exit_status";
	vars->i = 0;
	vars->counter = 0;
	vars->helper_index = 0;
	vars->i = 0;
	vars->d = 0;
	pipe->cmd_exec = NULL;
	pipe->current_result = 1;
	pipe->e_fd = open(vars->temp_exit_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipe->e_fd < 0)
	{
		printf("error\n");
	}
}

void	execute_middle_command_even(t_pipe *pipe, t_cmds *cmd, t_vars *vars)
{
	if (cmd[vars->counter].red_len > 0)
		check_exec_redirect(cmd, pipe, 1, vars->counter);
	if (input_check(cmd, pipe, vars->counter) == 0)
	{
		if (dup2(pipe->pipe_fds[1][0], STDIN_FILENO) == -1)
		{
			printf("error12\n");
			exit(0);
		}
	}
	if (output_check(cmd, pipe, vars->counter) == 0)
	{
		if (dup2(pipe->pipe_fds[0][1], STDOUT_FILENO) == -1)
		{
			printf("error14\n");
			exit(0);
		}
	}
}

void	exit_code_pipes(t_pipe *pipe, t_vars *vars)
{
	char	*s;
	int		fd;

	(void)pipe;
	s = NULL;
	close(pipe->e_fd);
	fd = open(vars->temp_exit_file, O_RDONLY);
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

void	child_exit(t_cmds *cmd, int j, t_pipe *pipe, int fd)
{
	g_exit_code = 127;
	write(2, cmd[j].cmd[0], ft_strlen(cmd[j].cmd[0]));
	write(2, ": command not found\n", 21);
	if (fd > 0)
	{
		ft_putnbr_fd(g_exit_code, fd);
	}
	close(fd);
	free_and_exit(pipe, cmd);
}
