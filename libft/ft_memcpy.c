/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:59:17 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/22 20:20:49 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	The memcpy() function copies n bytes from memory area src to memory area dst.
	If dst and src overlap, behavior is undefined. Applications in which dst and
	src might overlap should use memmove(3) instead.
*/

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t nbytes)
{
	size_t				i;
	unsigned char		*dst_ch;
	const unsigned char	*src_ch;

	if (!dst && !src)
		return (NULL);
	dst_ch = dst;
	src_ch = src;
	i = 0;
	while (i < nbytes)
	{
		dst_ch[i] = src_ch[i];
		i++;
	}
	return (dst);
}
