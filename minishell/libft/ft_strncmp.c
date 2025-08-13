/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:55:34 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/08/05 07:57:38 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	int				c;
	int				k;
	unsigned char	*ns1;
	unsigned char	*ns2;

	if (!s1 || !s2 || s1[0] == '\0' || s2[0] == '\0')
		return (-1);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (1);
	ns1 = (unsigned char *)s1;
	ns2 = (unsigned char *)s2;
	i = -1;
	c = ft_strlen(s1) - 1;
	k = ft_strlen(s2) - 1;
	while (++i < n && c >= 0 && k >= 0)
	{
		if (ns1[c] != ns2[k])
			return (ns1[c] - ns2[k]);
		c--;
		k--;
	}
	if (i != n)
		return (ns1[c] - ns2[c]);
	return (0);
}
