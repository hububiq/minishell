/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:59:41 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/16 12:57:46 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	cpy_forward(unsigned char *dst_ch,
		const unsigned char *src_ch, size_t nbytes)
{
	size_t	i;

	i = 0;
	while (i < nbytes)
	{
		dst_ch[i] = src_ch[i];
		i++;
	}
}

static void	cpy_backward(unsigned char *dst_ch,
		const unsigned char *src_ch, size_t nbytes)
{
	size_t	i;

	i = nbytes;
	while (i > 0)
	{
		i--;
		dst_ch[i] = src_ch[i];
	}
}

void	*ft_memmove(void *dst, const void *src, size_t nbytes)
{
	unsigned char		*dst_ch;
	const unsigned char	*src_ch;

	dst_ch = dst;
	src_ch = src;
	if (dst_ch == src_ch || nbytes == 0)
		return (dst);
	else if (dst_ch < src_ch)
		cpy_forward(dst_ch, src_ch, nbytes);
	else if (dst_ch > src_ch)
		cpy_backward(dst_ch, src_ch, nbytes);
	return (dst);
}
