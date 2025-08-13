/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:01:05 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:59:05 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command(t_cmds *cmds, t_pipe *pipe)
{
	if (cmds[0].cmd)
	{
		if (check_builtin(cmds, pipe) == 1)
		{
			check_other(cmds, pipe);
		}
		free_all(pipe, cmds);
	}
}

int	check_builtin(t_cmds *cmds, t_pipe *pipe)
{
	int	x;

	x = 0;
	if (cmds[x].cmd && cmds->cmd_len == 1)
	{
		if (ft_strncmp(cmds[x].cmd[0], "exit", 4) == 0)
			return (ft_exit(pipe, cmds), 0);
		else if (ft_strncmp(cmds[x].cmd[0], "cd", 2) == 0)
			return (ft_cd(cmds, x, 0, pipe), 0);
		else if (ft_strncmp(cmds[x].cmd[0], "export", 6) == 0)
			return (ft_export(pipe, cmds, x, 0), 0);
		else if (ft_strncmp(cmds[x].cmd[0], "unset", 5) == 0)
			return (ft_unset(cmds, x, 0, pipe), 0);
		else
		{
			ft_tolower(cmds[x].cmd[0]);
			if (ft_strncmp(cmds[x].cmd[0], "echo", 4) == 0)
				return (ft_echo(cmds, x, 0, pipe), 0);
			else if (ft_strncmp(cmds[x].cmd[0], "pwd", 3) == 0)
				return (ft_pwd(cmds, pipe, 0), 0);
			else if (ft_strncmp(cmds[x].cmd[0], "env", 3) == 0)
				return (ft_env(cmds, pipe, 0), 0);
		}
	}
	return (1);
}

void	check_other(t_cmds *cmds, t_pipe *pipe)
{
	pipe->index = 0;
	pipe->loop_counter = 0;
	update_env(pipe);
	get_path(pipe->tmp_env, pipe);
	if (cmds->cmd_len == 1)
	{
		pipe->character = 1;
		pipe->index = 0;
		normal_exec(cmds, pipe);
	}
	else if (cmds->cmd_len > 1)
		multiple_pipes(cmds, pipe);
	if (pipe->m_path)
	{
		free_strings(pipe->m_path);
		pipe->m_path = NULL;
	}
}

int	check_for_redirction(t_cmds *cmds, t_pipe *pipe)
{
	int	i;

	i = 0;
	pipe->index = 0;
	pipe->loop_counter = 0;
	while (pipe->index < cmds->cmd_len)
	{
		pipe->loop_counter = 0;
		while (cmds[pipe->index].cmd[pipe->loop_counter])
		{
			i = 0;
			while (cmds[pipe->index].cmd[pipe->loop_counter][i])
			{
				if (cmds[pipe->index].cmd[pipe->loop_counter][i] == '>'
					|| cmds[pipe->index].cmd[pipe->loop_counter][i] == '<')
					return (1);
				i++;
			}
			pipe->loop_counter++;
		}
		pipe->index++;
	}
	return (0);
}

void	normal_exec(t_cmds *cmds, t_pipe *pipe)
{
	if (!ft_strchr(cmds[0].cmd[0], '.') && ft_strchr(cmds[0].cmd[0], '/'))
	{
		if (check_if_file(cmds) == 1)
			return ;
	}
	else if (ft_strchr(cmds[0].cmd[0], '.') && ft_strchr(cmds[0].cmd[0], '/'))
	{
		if (check_dir(cmds) == 1)
			return ;
	}
	if (check_heredoc(cmds, pipe) == 1)
		exec_heredoc(cmds, pipe, 0);
	pipe->cmd_exec = check_command_existence(cmds[0].cmd[0], pipe->m_path);
	pipe->index = fork();
	if (pipe->index == 0)
	{
		if (cmds[0].red_len > 0)
			output_red(cmds, pipe, pipe->cmd_exec);
		if (!pipe->cmd_exec)
			error_in_exec(pipe, cmds);
		else if (execve(pipe->cmd_exec, cmds[0].cmd, pipe->tmp_env) < 0)
			execve_error(cmds, pipe);
	}
	waitpid(pipe->index, &pipe->status, 0);
	exit_status(pipe);
}
