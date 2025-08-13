/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:03 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 16:59:03 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*i;

	if ((size != 0 && count > SIZE_MAX / size))
		return (NULL);
	i = malloc((count * size));
	if (!i)
		return (NULL);
	ft_bzero(i, (count * size));
	return (i);
}
