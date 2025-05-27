/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:03:04 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/23 10:52:48 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	The toupper() function converts a lower-case letter to the
	corresponding upper-case letter. The argument must be
	representable as an unsigned char or the value of EOF.

	If the argument is a lower-case letter, the toupper() function
	returns the corresponding upper-case letter if there is one;
	otherwise, the argument is returned unchanged.
*/

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
