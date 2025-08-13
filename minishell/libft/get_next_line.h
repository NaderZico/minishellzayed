/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:54:13 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 07:51:10 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>

typedef struct s_var
{
	int		i;
	int		d;
	char	*tmp2;
	char	*s;
	char	*f;
}	t_var;

typedef struct read_vars
{
	char			*tmp;
	char			*str;
	int				n_pos;
}	t_rvars;

char	*get_next_line(int fd);

size_t	ft_strlen_and_ft_strchr(char *str, int c, int n);

char	*ft_strdup_get(char *s1);

char	*ft_strjoin_get(char *s1, char *s2);

char	*joining(char *str, char *tmp, int j);

char	*rest_less(char *buf, int j);

char	*ft_substr_get(char *s, unsigned int start, size_t len);

char	*saving(char *buf, int d);

char	*handle(char *buf, int j);

void	*check_eof(char **str, char **buf, t_var *vars, int p);
#endif