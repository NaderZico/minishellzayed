/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:01:23 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 17:01:23 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	output_red(t_cmds *cmds, t_pipe *pipe, char *cmd)
{
	(void)cmd;
	check_exec_redirect(cmds, pipe, 1, 0);
}

void	echo_new_line(t_cmds *cmds, int x, int y, t_pipe *pipe)
{
	(void)pipe;
	while (cmds[x].cmd[y])
	{
		printf("%s", cmds[x].cmd[y]);
		if (cmds[x].cmd[y + 1])
			printf(" ");
		y++;
	}
	printf("\n");
}

void	echo_flag(t_cmds *cmds, int x, int y, t_pipe *pipe)
{
	(void)pipe;
	y += 1;
	if (!cmds[x].cmd[y])
		return ;
	while (cmds[x].cmd[y + 1])
	{
		printf("%s ", cmds[x].cmd[y]);
		y++;
	}
	printf("%s", cmds[x].cmd[y]);
}

int	heredoc_condition(int fd)
{
	if (fd == 0)
		return (0);
	return (fd);
}

void	print_error(int i, int j, t_cmds *cmds)
{
	write(2, cmds[i].cmd[j], ft_strlen(cmds[i].cmd[j]));
	write (2, " : not a valid identifier\n", 26);
}
