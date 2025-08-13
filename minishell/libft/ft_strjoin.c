/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:56:03 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 16:56:03 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	i = 0;
	j = 0;
	str = malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(char) + 1);
	if (!str)
		return (NULL);
	if (s1 != NULL)
		while (s1[i] != '\0')
			str[j++] = s1[i++];
	i = 0;
	if (s2 != NULL)
		while ((s2[i] != '\0'))
			str[j++] = s2[i++];
	str[j] = '\0';
	if (*str == '\0')
		return (NULL);
	return (str);
}
