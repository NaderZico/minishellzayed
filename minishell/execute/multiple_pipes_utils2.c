/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:00:06 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:53:12 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtins_pipes(t_cmds *pipeCommands, t_pipe *pipe, int fd, int j)
{
	if (pipeCommands[j].cmd)
	{
		if (ft_strncmp(pipeCommands[j].cmd[0], "exit", 4) == 0)
			return (ft_exit(pipe, pipeCommands), 0);
		else if (ft_strncmp(pipeCommands[j].cmd[0], "cd", 2) == 0)
			return (execute_cd_with_pipes(pipeCommands, j, fd, pipe), 0);
		else if (ft_strncmp(pipeCommands[j].cmd[0], "export", 6) == 0)
			return (export_pipe_commands(pipe, pipeCommands, j, fd), 0);
		else if (ft_strncmp(pipeCommands[j].cmd[0], "unset", 5) == 0)
			return (ft_unset_p(pipeCommands, j, fd, pipe), 0);
		else
		{
			ft_tolower(pipeCommands[j].cmd[0]);
			if (ft_strncmp(pipeCommands[j].cmd[0], "echo", 4) == 0)
				return (execute_echo(pipeCommands, j, fd, pipe), 0);
			else if (ft_strncmp(pipeCommands[j].cmd[0], "pwd", 3) == 0)
				return (execute_print_working_directory(pipeCommands, pipe, fd),
					0);
			else if (ft_strncmp(pipeCommands[j].cmd[0], "env", 3) == 0)
				return (execute_env_command(pipeCommands, pipe, fd), 0);
		}
	}
	return (1);
}

int	input_check(t_cmds *cmd, t_pipe *pipe, int j)
{
	pipe->index = 0;
	while (pipe->index < cmd[j].red_len)
	{
		if (cmd[j].outs[pipe->index].flag == 0
			|| cmd[j].outs[pipe->index].flag == 3)
			return (1);
		pipe->index++;
	}
	return (0);
}

int	output_check(t_cmds *cmd, t_pipe *pipe, int j)
{
	pipe->index = 0;
	while (pipe->index < cmd[j].red_len)
	{
		if (cmd[j].outs[pipe->index].flag == 1
			|| cmd[j].outs[pipe->index].flag == 2)
			return (1);
		pipe->index++;
	}
	return (0);
}

void	exit_once(t_cmds *cmd, t_pipe *pipe)
{
	(void)cmd;
	if (pipe->env_variables)
		free_list(&pipe->env_variables);
	if (pipe->m_export)
		free_list(&pipe->m_export);
	if (pipe->tmp_env)
		free_strings(pipe->tmp_env);
	if (pipe->m_path != NULL)
		free_strings(pipe->m_path);
	if (pipe->cmd_exec)
		free(pipe->cmd_exec);
	free_all(pipe, cmd);
	close(pipe->fdin);
	close(pipe->fdout);
	exit(g_exit_code);
}

void	check_and_exit(t_pipe *pipe)
{
	if (pipe->pipe_flag_read == 1)
	{
		close(pipe->pipe_fds[0][0]);
		close(pipe->pipe_fds[0][1]);
	}
	if (pipe->pipe_flag_read_index == 1)
	{
		close(pipe->pipe_fds[1][0]);
		close(pipe->pipe_fds[1][1]);
	}
}
