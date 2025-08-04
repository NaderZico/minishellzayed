/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:45:28 by abdamoha          #+#    #+#             */
/*   Updated: 2025/08/04 12:19:25 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtins_pipes(t_cmds *pipeCommands, t_pipe *c, int fd, int j)
{
	if (pipeCommands[j].cmd)
	{
		if (ft_strncmp(pipeCommands[j].cmd[0], "exit", 4) == 0)
			return (ft_exit(c, pipeCommands), 0);
		else if (ft_strncmp(pipeCommands[j].cmd[0], "cd", 2) == 0)
			return (execute_cd_with_pipes(pipeCommands, j, fd, c), 0);
		else if (ft_strncmp(pipeCommands[j].cmd[0], "export", 6) == 0)
			return (export_pipe_commands(c, pipeCommands, j, fd), 0);
		else if (ft_strncmp(pipeCommands[j].cmd[0], "unset", 5) == 0)
			return (ft_unset_p(pipeCommands, j, fd, c), 0);
		else
		{
			ft_tolower(pipeCommands[j].cmd[0]);
			if (ft_strncmp(pipeCommands[j].cmd[0], "echo", 4) == 0)
				return (execute_echo(pipeCommands, j, fd, c), 0);
			else if (ft_strncmp(pipeCommands[j].cmd[0], "pwd", 3) == 0)
				return (execute_print_working_directory(pipeCommands, c, fd),
					0);
			else if (ft_strncmp(pipeCommands[j].cmd[0], "env", 3) == 0)
				return (execute_env_command(pipeCommands, c, fd), 0);
		}
	}
	return (1);
}

int	input_check(t_cmds *p, t_pipe *c, int j)
{
	c->index = 0;
	while (c->index < p[j].redir_count)
	{
		if (p[j].outs[c->index].flag == 0 || p[j].outs[c->index].flag == 3)
			return (1);
		c->index++;
	}
	return (0);
}

int	output_check(t_cmds *p, t_pipe *c, int j)
{
	c->index = 0;
	while (c->index < p[j].redir_count)
	{
		if (p[j].outs[c->index].flag == 1 || p[j].outs[c->index].flag == 2)
			return (1);
		c->index++;
	}
	return (0);
}

void	exit_once(t_cmds *p, t_pipe *c)
{
	(void)p;
	if (c->env_variables)
		free_list(&c->env_variables);
	if (c->m_export)
		free_list(&c->m_export);
	if (c->tmp_env)
		free_strings(c->tmp_env);
	if (c->m_path != NULL)
		free_strings(c->m_path);
	if (c->cmd_exec)
		free(c->cmd_exec);
	free_all(c, p);
	close(c->fdin);
	close(c->fdout);
	exit(g_exit_code);
}

void	check_and_exit(t_pipe *c)
{
	if (c->pipe_flag_read == 1)
	{
		close(c->pipe_fds[0][0]);
		close(c->pipe_fds[0][1]);
	}
	if (c->pipe_flag_read_index == 1)
	{
		close(c->pipe_fds[1][0]);
		close(c->pipe_fds[1][1]);
	}
}
