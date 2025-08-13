/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:58:43 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 16:58:43 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isexportable(char *str, int len)
{
	int		i;

	i = 0;
	(void)len;
	if (str)
	{
		if (ft_isdigit(str[0]) || (!ft_isalpha(str[0]) && str[0] != '_'))
			return (1);
		i++;
		while (str[i] && i < len)
		{
			if (!ft_isalnum(str[i]) && str[i] != '_')
			{
				return (1);
			}
			i++;
		}
	}
	return (0);
}

int	ft_isdigit(int f)
{
	return ((f >= '0' && f <= '9'));
}
