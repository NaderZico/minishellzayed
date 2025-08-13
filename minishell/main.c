/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:28:32 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/13 16:01:34 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code = 0;

static int	is_blank(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

static int	validate_input(t_pipe *pipe, t_cmds *cmds, char *read)
{
	if (is_blank(read))
		return (1);
	if (!is_valid_redir(read))
	{
		printf("syntax error near unexpected token \n");
		add_history(read);
		g_exit_code = 258;
		return (1);
	}
	else if (!validate_pipes(pipe, read, cmds))
	{
		printf("Error\n");
		add_history(read);
		g_exit_code = 258;
		return (1);
	}
	return (0);
}

static void	run_command(t_pipe *pipe, t_cmds *cmds)
{
	pipe->fdin = dup(0);
	pipe->fdout = dup(1);
	execute_command(cmds, pipe);
	dup2(pipe->fdin, STDIN_FILENO);
	dup2(pipe->fdout, STDOUT_FILENO);
	close(pipe->fdin);
	close(pipe->fdout);
}

static void	main_init(t_pipe *pipe)
{
	pipe->m_path = NULL;
	pipe->fdin = 0;
	pipe->fdout = 0;
	pipe->character = 0;
	pipe->current_result = 0;
	pipe->pipe_flag_read = 0;
	pipe->pipe_flag_read_index = 0;
	pipe->e_fd = 0;
}

int	main(int ac, char **av, char **envp)
{
	t_pipe	pipe;
	t_cmds	*cmds;
	char	*read;

	(void)av;
	(void)ac;
	cmds = NULL;
	get_env(&pipe, envp);
	fill_export(&pipe);
	g_exit_code = 0;
	main_init(&pipe);
	while (1)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		read = readline("minishell$ ");
		if (!read)
			return (free_and_exit_2(&pipe, cmds), printf("exit\n"), 0);
		if (validate_input(&pipe, cmds, read))
			continue ;
		save_cmd_files(&pipe, &cmds);
		run_command(&pipe, cmds);
		add_history(read);
	}
}
