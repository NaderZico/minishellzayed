/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:28 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:53:42 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_first_command(t_pipe *pipes, t_cmds *cmds, t_vars *v)
{
	close(pipes->e_fd);
	if (cmds[v->counter].red_len > 0)
		check_exec_redirect(cmds, pipes, 1, v->counter);
	else
		dup2(pipes->pipe_fds[0][1], STDOUT_FILENO);
	if (input_check(cmds, pipes, v->counter))
		dup2(pipes->pipe_fds[0][1], STDOUT_FILENO);
	if (builtins_pipes(cmds, pipes, pipes->pipe_fds[0][1], v->counter) == 0)
		free_and_exit(pipes, cmds);
	close(pipes->pipe_fds[0][1]);
	close(pipes->pipe_fds[0][0]);
	pipes->cmd_exec = check_command_existence(cmds[v->counter].cmd[0],
			pipes->m_path);
	if (!pipes->cmd_exec)
		child_exit(cmds, v->counter, pipes, 0);
	if (execve(pipes->cmd_exec, cmds[v->counter].cmd, NULL) < 0)
	{
		write(2, cmds[v->counter].cmd[0], ft_strlen(cmds[v->counter].cmd[0]));
		write(2, ": command not found\n", 21);
		free(pipes->cmd_exec);
		closing_fds(pipes);
		g_exit_code = 126;
		free_and_exit(pipes, cmds);
	}
}

void	second_cmd(t_pipe *pipes, t_vars *v, t_cmds *cmds)
{
	if (v->i % 2 == 0 && v->counter == 1 && input_check(cmds, pipes,
			v->counter) == 0)
	{
		if (dup2(pipes->pipe_fds[0][0], STDIN_FILENO) == -1)
		{
			printf("error3\n");
			exit(0);
		}
		close(pipes->pipe_fds[0][0]);
		close(pipes->pipe_fds[0][1]);
	}
	else if (v->i % 2 == 1 && input_check(cmds, pipes, v->counter) == 0)
	{
		dup2(pipes->pipe_fds[0][0], STDIN_FILENO);
		close(pipes->pipe_fds[0][1]);
		close(pipes->pipe_fds[0][0]);
	}
	else
	{
		if (input_check(cmds, pipes, v->counter) == 0)
			dup2(pipes->pipe_fds[1][0], STDIN_FILENO);
		close(pipes->pipe_fds[1][1]);
		close(pipes->pipe_fds[1][0]);
	}
}

void	execute_last_command(t_pipe *pipes, t_cmds *cmds, t_vars *v)
{
	if (cmds[v->counter].red_len > 0)
		check_exec_redirect(cmds, pipes, 1, v->counter);
	second_cmd(pipes, v, cmds);
	if (builtins_pipes(cmds, pipes, 1, v->counter) == 0)
		last_exit_e(v, cmds, pipes);
	pipes->cmd_exec = check_command_existence(cmds[v->counter].cmd[0],
			pipes->m_path);
	if (!pipes->cmd_exec)
		child_exit(cmds, v->counter, pipes, pipes->e_fd);
	else
	{
		close(pipes->fdin);
		close(pipes->fdout);
	}
	if (execve(pipes->cmd_exec, cmds[v->counter].cmd, NULL) < 0)
	{
		write(2, cmds[v->counter].cmd[0], ft_strlen(cmds[v->counter].cmd[0]));
		write(2, ": command not found\n", 21);
		free(pipes->cmd_exec);
		g_exit_code = 126;
		ft_putnbr_fd(g_exit_code, pipes->e_fd);
		close(pipes->e_fd);
		free_and_exit(pipes, cmds);
	}
}

void	execute_middle_command_odd(t_pipe *pipes, t_cmds *cmds, t_vars *v)
{
	if (cmds[v->counter].red_len > 0)
		check_exec_redirect(cmds, pipes, 1, v->counter);
	if (input_check(cmds, pipes, v->counter) == 0 && output_check(cmds, pipes,
			v->counter) == 0)
	{
		if (input_check(cmds, pipes, v->counter) == 0)
			dup2(pipes->pipe_fds[0][0], STDIN_FILENO);
		if (output_check(cmds, pipes, v->counter) == 0)
			dup2(pipes->pipe_fds[1][1], STDOUT_FILENO);
	}
	else
	{
		if (input_check(cmds, pipes, v->counter) == 0)
			dup2(pipes->pipe_fds[0][0], STDIN_FILENO);
		if (output_check(cmds, pipes, v->counter) == 0)
		{
			if (dup2(pipes->pipe_fds[1][1], STDOUT_FILENO) == -1)
			{
				printf("error10\n");
				exit(0);
			}
		}
		close(pipes->pipe_fds[0][0]);
		close(pipes->pipe_fds[1][1]);
	}
}

void	multiple_pipes(t_cmds *cmds, t_pipe *pipes)
{
	t_vars	variables;

	initialize_variables(&variables, pipes);
	while (variables.counter < cmds->cmd_len)
	{
		if (variables.i % 2 == 0)
		{
			pipes->pipe_flag_read = 1;
			pipe(pipes->pipe_fds[0]);
		}
		else if (variables.i % 2 == 1)
		{
			pipes->pipe_flag_read_index = 1;
			pipe(pipes->pipe_fds[1]);
		}
		if (check_heredoc(cmds, pipes) == 1)
			exec_heredoc(cmds, pipes, variables.counter);
		pipes->pid = fork();
		before_cmd(pipes, cmds, &variables);
		closing_pipe(pipes, cmds, &variables);
		variables.i++;
		variables.counter++;
	}
	closing_fds(pipes);
	wait_pipes(&variables, pipes, cmds);
}
