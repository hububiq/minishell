/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:05:57 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/14 21:01:05 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char		ch;
	const unsigned char	*str;
	size_t				i;

	ch = c;
	str = s;
	i = 0;
	while (i < n)
	{
		if (str[i] == ch)
			return ((char *) &str[i]);
		i++;
	}
	return (NULL);
}
