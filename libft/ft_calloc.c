/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:54:13 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/23 15:33:33 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	The calloc() function contiguously allocates enough space for count objects 
	that are size bytes of memory each and returns a pointer to the allocated 
	memory. The allocated memory is filled with bytes of value zero.

	If successful, calloc() function returns a pointer to allocated memory.
	If there is an error, they return a NULL pointer and set errno to ENOMEM.
*/

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*mem;

	if (!count || !size)
		return (malloc(1));
	if (count && ((count * size) / count != size))
		return (NULL);
	mem = (void *)malloc(count * size);
	if (!mem)
		return (NULL);
	ft_bzero(mem, size * count);
	return (mem);
}
