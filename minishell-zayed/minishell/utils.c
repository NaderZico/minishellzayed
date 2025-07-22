/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakhalil <nakhalil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:33:37 by nakhalil          #+#    #+#             */
/*   Updated: 2025/05/24 18:38:34 by nakhalil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * safe_malloc
 *   malloc wrapper that exits(1) on failure.
 */
void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		write(2, "Malloc error\n", 13);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

/**
 * ft_realloc
 *   Grow or shrink a block at ptr (of old_size) to new_size,
 *   exiting on allocation failure.
 */
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (safe_malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = safe_malloc(new_size);
	ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}

/*
** ft_extend_arr
**   Append new_str to a NULL-terminated array of strings.
**   Returns the new array (old one is freed).  Exits on alloc failure.
*/
char	**ft_extend_arr(char **arr, char *new_str)
{
	int		count;
	int		i;
	char	**new;

	count = 0;
	while (arr && arr[count])
		count++;
	new = safe_malloc(sizeof(char *) * (count + 2));
	i = 0;
	while (i < count)
	{
		new[i] = arr[i];
		i++;
	}
	new[count] = ft_strdup(new_str);
	new[count + 1] = NULL;
	free(arr);
	return (new);
}

void	append_char(char **buf, size_t *cap, size_t *len, char c)
{
	if (*len + 1 >= *cap)
	{
		*buf = ft_realloc(*buf, *cap, *cap * 2);
		*cap *= 2;
	}
	(*buf)[(*len)++] = c;
	(*buf)[*len] = '\0';
}

void	handle_error(t_error err, t_data *data, char *context)
{
		const char *tok_val = data->tokens[data->error_pos].value;

	if (err == ERR_SYNTAX)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		ft_putstr_fd((char *)tok_val, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (context)
		{
			ft_putstr_fd(context, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
		}
		if (err == ERR_MALLOC)
			ft_putendl_fd("memory allocation failed", STDERR_FILENO);
		else if (err == ERR_CMD_NOT_FOUND)
			ft_putendl_fd("command not found", STDERR_FILENO);
		else
			ft_putendl_fd("unknown error", STDERR_FILENO);
	}
	data->last_status = (err == ERR_SIGINT) ? 130 : err;
}
