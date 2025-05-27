/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:09:19 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/22 23:40:29 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	numlen(int n)
{
	int		len;
	long	nbr;

	nbr = n;
	len = 1;
	if (n < 0)
	{
		nbr = -nbr;
		len++;
	}
	while (nbr >= 10)
	{
		nbr = nbr / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	nbr;
	int		i;

	len = numlen(n);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	nbr = n;
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	str[len] = '\0';
	i = 1;
	while (nbr >= 10)
	{
		str[len - i] = (nbr % 10) + '0';
		nbr = nbr / 10;
		i++;
	}
	str[len - i] = (nbr % 10) + '0';
	return (str);
}
