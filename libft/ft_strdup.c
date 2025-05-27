/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:08:47 by mdziadko          #+#    #+#             */
/*   Updated: 2025/05/09 18:07:11 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	The strdup() function allocates sufficient memory for a copy of the string 
	s1, does the copy, and returns a pointer to it. The pointer may subsequently
	be used as an argument to the function free(3).
*/

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*dst;
	int		i;

	dst = malloc((ft_strlen(src) + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
