/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:53:47 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/14 16:32:42 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*replace_substr(char *str, int start, int len, char *replace)
{
	char	*new_str;
	int		i;
	int		j;

	if (!str || start + len > (int)ft_strlen(str))
		return (NULL);
	new_str = malloc(sizeof(char) * (ft_strlen(str) - len + ft_strlen(replace)
				+ 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (i < start)
		new_str[j++] = str[i++];
	while (*replace)
		new_str[j++] = *replace++;
	while (str[i + len])
	{
		new_str[j] = str[i + len];
		i++;
		j++;
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

static int	expand_env_var(char **str, char *expanded, t_vars *var)
{
	expanded = NULL;
	var->i++;
	if ((*str)[var->i] == '?' && (*str)[var->i - 1] == '$')
	{
		expanded = ft_itoa(g_exit_code);
		*str = replace_substr(*str, var->i - 1, 2, expanded);
		var->i += ft_strlen(expanded) - 2;
		free(expanded);
		return (1);
	}
	if (!ft_isalpha((*str)[var->i]) && (*str)[var->i] != '_')
		return (1);
	var->start = var->i;
	var->len = 0;
	while (ft_isalnum((*str)[var->i]) || (*str)[var->i] == '_')
	{
		var->len++;
		var->i++;
	}
	return (0);
}

static int	process_expansion(char **str, char **tmp, t_vars *var, t_pipe *pipe)
{
	char	*expanded;

	expanded = NULL;
	if (*tmp)
	{
		expanded = get_env_value(*tmp, pipe);
		if (expanded)
		{
			*str = replace_substr(*str, var->i - var->len - 1, var->len + 1,
					expanded);
			var->i += ft_strlen(expanded) - var->len - 1;
		}
		else if (!expanded)
		{
			*str = replace_substr(*str, var->i - var->len - 1, var->len + 1,
					"");
			var->i -= var->len + 1;
		}
		free(*tmp);
		return (1);
	}
	return (0);
}

void	expand_dollar_vars(char **str, t_pipe *pipe)
{
	t_vars	var;
	char	*tmp;
	char	*expanded;

	if (!str || !*str)
		return ;
	var.len = 0;
	expanded = NULL;
	var.i = 0;
	var.in_d_quotes = 0;
	var.in_quotes = 0;
	while ((*str)[var.i])
	{
		update_quote_state((*str)[var.i], &var.quote_char);
		if ((*str)[var.i] == '$' && !var.in_quotes && (*str)[var.i + 1])
		{
			if (expand_env_var(str, expanded, &var))
				continue ;
			tmp = ft_substr(*str, var.start, var.len);
			if (process_expansion(str, &tmp, &var, pipe))
				continue ;
		}
		var.i++;
	}
}
