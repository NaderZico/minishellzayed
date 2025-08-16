/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:53:39 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/16 16:22:02 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	remove_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (i < start || i >= len)
		{
			s[j] = s[i];
			j++;
		}
		i++;
	}
	s[j] = '\0';
}

static void	extract_file_name(char *str, char **file_name, int i, t_vars *v)
{
	int	start;
	int	in_q;

	start = i;
	in_q = 0;
	while (str[i])
	{
		update_quote_state(str[i], &in_q);
		if ((str[i] == ' ' || str[i] == '\t') && !in_q)
			break ;
		i++;
	}
	v->i = i;
	*file_name = ft_substr(str, start, i - start);
}

static int	set_redirs(t_pipe *p, t_vars *v, int *redir_pos)
{
	int	flag;

	flag = -1;
	v->start = *redir_pos - 1;
	if (p->cmds[v->cmd_i][*redir_pos + 1] == '>' || p->cmds[v->cmd_i][*redir_pos
		+ 1] == '<')
	{
		if (p->cmds[v->cmd_i][*redir_pos + 1] == '>')
			flag = APPEND;
		else if (p->cmds[v->cmd_i][*redir_pos + 1] == '<')
			flag = HERE_DOC;
		*redir_pos += 2;
	}
	else if (p->cmds[v->cmd_i][*redir_pos] == '>')
	{
		flag = OUT_FILE;
		(*redir_pos)++;
	}
	else if (p->cmds[v->cmd_i][*redir_pos] == '<')
	{
		flag = IN_FILE;
		(*redir_pos)++;
	}
	return (flag);
}

static void	process_redirs(t_pipe *p, t_cmds *cmds, t_vars *v)
{
	int	redir_i;
	int	redir_pos;
	int	flag;

	redir_i = 0;
	redir_pos = -1;
	while (p->cmds[v->cmd_i][++redir_pos])
	{
		update_quote_state(p->cmds[v->cmd_i][redir_pos], &v->quote_char);
		if ((p->cmds[v->cmd_i][redir_pos] == '>'
			|| p->cmds[v->cmd_i][redir_pos] == '<') && !v->quote_char)
		{
			flag = set_redirs(p, v, &redir_pos);
			cmds[v->cmd_i].outs[redir_i].flag = flag;
			extract_file_name(p->cmds[v->cmd_i],
				&cmds[v->cmd_i].outs[redir_i].file_name, redir_pos + 1, v);
			clean_quotes(cmds[v->cmd_i].outs[redir_i].file_name);
			remove_substr(p->cmds[v->cmd_i], v->start, v->i);
			redir_pos = v->start - 1;
			redir_i++;
		}
	}
}

void	save_cmd_files(t_pipe *p, t_cmds **tmp)
{
	t_cmds	*cmds;
	t_vars	v;

	v.start = 0;
	v.quote_char = 0;
	v.arg_i = 0;
	v.cmd_i = -1;
	*tmp = malloc(sizeof(t_cmds) * p->total_cmds);
	cmds = *tmp;
	cmds->cmd_len = p->total_cmds;
	cmds->red_len = 0;
	while (++v.cmd_i < p->total_cmds)
	{
		cmds[v.cmd_i].red_len = count_redirs(p->cmds[v.cmd_i]);
		if (cmds[v.cmd_i].red_len)
			cmds[v.cmd_i].outs = malloc(sizeof(t_redirect)
					* cmds[v.cmd_i].red_len);
		process_redirs(p, cmds, &v);
		cmds[v.cmd_i].cmd = ft_split(p->cmds[v.cmd_i], ' ');
		v.arg_i = 0;
		while (cmds[v.cmd_i].cmd[v.arg_i])
			clean_quotes(cmds[v.cmd_i].cmd[v.arg_i++]);
		v.arg_i = 0;
	}
}
