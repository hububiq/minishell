/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:58:28 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/29 10:04:30 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t nbytes)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	ch;

	ch = c;
	ptr = s;
	i = 0;
	if (!nbytes)
		return (s);
	while (i < nbytes)
	{
		ptr[i] = ch;
		i++;
	}
	return (s);
}
