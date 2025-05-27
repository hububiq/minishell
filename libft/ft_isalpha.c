/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:53:57 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/22 16:38:05 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	The isalpha() function tests for any character for which isupper(3) or
	islower(3) is true. The value of the argument must be representable as an
	unsigned char or the value of EOF.
*/

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
