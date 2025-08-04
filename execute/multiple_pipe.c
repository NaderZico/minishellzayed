/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 16:00:16 by abdamoha          #+#    #+#             */
/*   Updated: 2025/08/04 12:19:25 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	first_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	close(c->e_fd);
	if (p[v->counter].redir_count > 0)
		check_exec_redirect(p, c, 1, v->counter);
	else
		dup2(c->pipe_fds[0][1], STDOUT_FILENO);
	if (input_check(p, c, v->counter))
		dup2(c->pipe_fds[0][1], STDOUT_FILENO);
	if (builtins_pipes(p, c, c->pipe_fds[0][1], v->counter) == 0)
		free_and_exit(c, p);
	close(c->pipe_fds[0][1]);
	close(c->pipe_fds[0][0]);
	c->cmd_exec = check_command_existence(p[v->counter].cmd[0], c->m_path);
	if (!c->cmd_exec)
		child_exit(p, v->counter, c, 0);
	if (execve(c->cmd_exec, p[v->counter].cmd, NULL) < 0)
	{
		write(2, p[v->counter].cmd[0],
			ft_strlen(p[v->counter].cmd[0]));
		write(2, ": command not found\n", 21);
		free(c->cmd_exec);
		closing_fds(c);
		g_exit_code = 126;
		free_and_exit(c, p);
	}
}

void	second_cmd(t_pipe *c, t_vars *v, t_cmds *p)
{
	if (v->i % 2 == 0 && v->counter == 1 && input_check(p, c,
			v->counter) == 0)
	{
		if (dup2(c->pipe_fds[0][0], STDIN_FILENO) == -1)
		{
			printf("error3\n");
			exit(0);
		}
		close(c->pipe_fds[0][0]);
		close(c->pipe_fds[0][1]);
	}
	else if (v->i % 2 == 1 && input_check(p, c, v->counter) == 0)
	{
		dup2(c->pipe_fds[0][0], STDIN_FILENO);
		close(c->pipe_fds[0][1]);
		close(c->pipe_fds[0][0]);
	}
	else
	{
		if (input_check(p, c, v->counter) == 0)
			dup2(c->pipe_fds[1][0], STDIN_FILENO);
		close(c->pipe_fds[1][1]);
		close(c->pipe_fds[1][0]);
	}
}

void	third2_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (p[v->counter].redir_count > 0)
		check_exec_redirect(p, c, 1, v->counter);
	second_cmd(c, v, p);
	if (builtins_pipes(p, c, 1, v->counter) == 0)
		last_exit_e(v, p, c);
	c->cmd_exec = check_command_existence(p[v->counter].cmd[0], c->m_path);
	if (!c->cmd_exec)
		child_exit(p, v->counter, c, c->e_fd);
	else
	{
		close(c->fdin);
		close(c->fdout);
	}
	if (execve(c->cmd_exec, p[v->counter].cmd, NULL) < 0)
	{
		write(2, p[v->counter].cmd[0],
			ft_strlen(p[v->counter].cmd[0]));
		write(2, ": command not found\n", 21);
		free(c->cmd_exec);
		g_exit_code = 126;
		ft_putnbr_fd(g_exit_code, c->e_fd);
		close(c->e_fd);
		free_and_exit(c, p);
	}
}

void	fourth_cmd(t_pipe *c, t_cmds *p, t_vars *v)
{
	if (p[v->counter].redir_count > 0)
		check_exec_redirect(p, c, 1, v->counter);
	if (input_check(p, c, v->counter) == 0 && output_check(p, c,
			v->counter) == 0)
	{
		if (input_check(p, c, v->counter) == 0)
			dup2(c->pipe_fds[0][0], STDIN_FILENO);
		if (output_check(p, c, v->counter) == 0)
			dup2(c->pipe_fds[1][1], STDOUT_FILENO);
	}
	else
	{
		if (input_check(p, c, v->counter) == 0)
			dup2(c->pipe_fds[0][0], STDIN_FILENO);
		if (output_check(p, c, v->counter) == 0)
		{
			if (dup2(c->pipe_fds[1][1], STDOUT_FILENO) == -1)
			{
				printf("error10\n");
				exit(0);
			}
		}
		close(c->pipe_fds[0][0]);
		close(c->pipe_fds[1][1]);
	}
}

void	multiple_pipes(t_cmds *commands, t_pipe *pipeConfig)
{
	t_vars	variables;

	initialize_variables(&variables, pipeConfig);
	while (variables.counter < commands->cmd_len)
	{
		if (variables.i % 2 == 0)
		{
			pipeConfig->pipe_flag_read = 1;
			pipe(pipeConfig->pipe_fds[0]);
		}
		else if (variables.i % 2 == 1)
		{
			pipeConfig->pipe_flag_read_index = 1;
			pipe(pipeConfig->pipe_fds[1]);
		}
		if (check_heredoc(commands, pipeConfig) == 1)
			exec_heredoc(commands, pipeConfig, variables.counter);
		pipeConfig->pid = fork();
		before_cmd(pipeConfig, commands, &variables);
		closing_pipe(pipeConfig, commands, &variables);
		variables.i++;
		variables.counter++;
	}
	closing_fds(pipeConfig);
	wait_pipes(&variables, pipeConfig, commands);
}
