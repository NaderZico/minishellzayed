/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:23:52 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/04 12:19:25 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command(t_cmds *p, t_pipe *c)
{
	if (p[0].cmd)
	{
		if (check_builtin(p, c) == 1)
		{
			check_other(p, c);
		}
		free_all(c, p);
	}
}

int	check_builtin(t_cmds *commandStruct, t_pipe *pipecommand)
{
	int	x;

	x = 0;
	if (commandStruct[x].cmd && commandStruct->cmd_len == 1)
	{
		if (ft_strncmp(commandStruct[x].cmd[0], "exit", 4) == 0)
			return (ft_exit(pipecommand, commandStruct), 0);
		else if (ft_strncmp(commandStruct[x].cmd[0], "cd", 2) == 0)
			return (ft_cd(commandStruct, x, 0, pipecommand), 0);
		else if (ft_strncmp(commandStruct[x].cmd[0], "export", 6) == 0)
			return (ft_export(pipecommand, commandStruct, x, 0), 0);
		else if (ft_strncmp(commandStruct[x].cmd[0], "unset", 5) == 0)
			return (ft_unset(commandStruct, x, 0, pipecommand), 0);
		else
		{
			ft_tolower(commandStruct[x].cmd[0]);
			if (ft_strncmp(commandStruct[x].cmd[0], "echo", 4) == 0)
				return (ft_echo(commandStruct, x, 0, pipecommand), 0);
			else if (ft_strncmp(commandStruct[x].cmd[0], "pwd", 3) == 0)
				return (ft_pwd(commandStruct, pipecommand, 0), 0);
			else if (ft_strncmp(commandStruct[x].cmd[0], "env", 3) == 0)
				return (ft_env(commandStruct, pipecommand, 0), 0);
		}
	}
	return (1);
}

void	check_other(t_cmds *commandList, t_pipe *pipe_structure)
{
	pipe_structure->index = 0;
	pipe_structure->loop_counter = 0;
	update_env(pipe_structure);
	get_path(pipe_structure->tmp_env, pipe_structure);
	if (commandList->cmd_len == 1)
	{
		pipe_structure->character = 1;
		pipe_structure->index = 0;
		normal_exec(commandList, pipe_structure);
	}
	else if (commandList->cmd_len > 1)
		multiple_pipes(commandList, pipe_structure);
	if (pipe_structure->m_path)
	{
		free_strings(pipe_structure->m_path);
		pipe_structure->m_path = NULL;
	}
}

int	check_for_redirction(t_cmds *p, t_pipe *c)
{
	int	counter;

	counter = 0;
	c->index = 0;
	c->loop_counter = 0;
	while (c->index < p->cmd_len)
	{
		c->loop_counter = 0;
		while (p[c->index].cmd[c->loop_counter])
		{
			counter = 0;
			while (p[c->index].cmd[c->loop_counter][counter])
			{
				if (p[c->index].cmd[c->loop_counter][counter] == '>'
					|| p[c->index].cmd[c->loop_counter][counter] == '<')
					return (1);
				counter++;
			}
			c->loop_counter++;
		}
		c->index++;
	}
	return (0);
}

void	normal_exec(t_cmds *p, t_pipe *c)
{
	if (!ft_strchr(p[0].cmd[0], '.') && ft_strchr(p[0].cmd[0], '/'))
	{
		if (check_if_file(p) == 1)
			return ;
	}
	else if (ft_strchr(p[0].cmd[0], '.') && ft_strchr(p[0].cmd[0], '/'))
	{
		if (check_dir(p) == 1)
			return ;
	}
	if (check_heredoc(p, c) == 1)
		exec_heredoc(p, c, 0);
	c->cmd_exec = check_command_existence(p[0].cmd[0], c->m_path);
	c->index = fork();
	if (c->index == 0)
	{
		if (p[0].redir_count > 0)
			output_red(p, c, c->cmd_exec);
		if (!c->cmd_exec)
			error_in_exec(c, p);
		else if (execve(c->cmd_exec, p[0].cmd, c->tmp_env) < 0)
			execve_error(p, c);
	}
	waitpid(c->index, &c->status, 0);
	exit_status(c);
}
