/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:01:11 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 08:58:10 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_cmds *cmd, int i, int fd, t_pipe *pipe)
{
	int	j;

	j = 0;
	(void)fd;
	if (cmd->red_len > 0 && pipe->current_result != 1)
		check_exec_redirect(cmd, pipe, 0, 0);
	if (!cmd[i].cmd[j + 1])
		return ;
	j = 1;
	while (cmd[i].cmd[j])
	{
		if (ft_isalpha_str(cmd[i].cmd[j]) == 0)
		{
			unset_cmp(&pipe->env_variables, cmd[i].cmd[j], pipe);
			unset_cmp(&pipe->m_export, cmd[i].cmd[j], pipe);
		}
		else
			print_error(i, j, cmd);
		j++;
	}
	pipe->env_count -= 1;
}

char	*check_command_existence(char *av, char **path)
{
	int		i;
	char	*str;
	char	*join;
	int		j;

	j = 0;
	i = ft_strlen(av);
	if (i == 0)
		return (NULL);
	if (ft_strchr(av, '/') != NULL)
	{
		return (backslash_case(av, i));
	}
	str = malloc(i + 2);
	str[0] = '/';
	i = 1;
	while (av[j] != '\0')
	{
		str[i] = av[j];
		j++;
		i++;
	}
	str[i] = '\0';
	join = check_with_access(path, str);
	return (join);
}

char	*check_with_access(char **path, char *str)
{
	int		i;
	char	*join;

	i = 0;
	join = NULL;
	if (path)
	{
		while (path[i] != NULL)
		{
			join = ft_strjoin(path[i], str);
			if (access(join, X_OK) != -1)
			{
				return (free(str), join);
			}
			i++;
			free(join);
			join = NULL;
		}
	}
	return (free(join), free(str), NULL);
}

char	*backslash_case(char *av, int i)
{
	int		j;
	char	*join;

	j = 0;
	join = malloc(i + 1);
	while (av[j] != '\0')
	{
		join[j] = av[j];
		j++;
	}
	join[j] = '\0';
	return (join);
}

void	last_sorting(t_pipe *cmd)
{
	char	*str;

	cmd->index = 0;
	cmd->loop_counter = 0;
	str = NULL;
	while (cmd->index < cmd->env_count - 1)
	{
		cmd->loop_counter = 0;
		while (cmd->loop_counter < cmd->env_count - 1 - cmd->index)
		{
			if (custom_strncmp(cmd->tmp_env[cmd->loop_counter],
					cmd->tmp_env[cmd->loop_counter + 1],
					len_till_equal(cmd->tmp_env[cmd->loop_counter])) > 0)
			{
				str = cmd->tmp_env[cmd->loop_counter];
				cmd->tmp_env[cmd->loop_counter] = cmd->tmp_env[cmd->loop_counter
					+ 1];
				cmd->tmp_env[cmd->loop_counter + 1] = str;
			}
			cmd->loop_counter++;
		}
		cmd->index++;
	}
}
