/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsid-ele <zsid-ele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:58:31 by zsid-ele          #+#    #+#             */
/*   Updated: 2025/07/30 16:58:31 by zsid-ele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countdigit(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	unsigned int	num;
	char			*s;
	size_t			len;

	len = countdigit(n);
	num = n;
	if (n < 0)
	{
		num = n * -1;
		len++;
	}
	s = malloc(sizeof(char) * (len + 1));
	if (s == NULL)
		return (NULL);
	s[len] = '\0';
	if (num == 0)
		s[--len] = '0';
	while (num)
	{
		s[--len] = num % 10 + '0';
		num = num / 10;
	}
	if (n < 0)
		s[0] = '-';
	return (s);
}
