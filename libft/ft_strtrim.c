/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:27:49 by mdziadko          #+#    #+#             */
/*   Updated: 2024/12/15 19:58:37 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s, char const *set)
{
	char	*str;
	int		end;
	int		start;

	if (!s)
		return (NULL);
	if (!set || !*set)
		return (ft_strdup(s));
	start = 0;
	end = ft_strlen(s) - 1;
	while (s[start] && ft_strchr(set, s[start]))
		start++;
	while (end >= start && ft_strrchr(set, s[end]))
		end--;
	if (start > end)
		return (ft_strdup(""));
	str = malloc((end - start + 2) * sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s[start], end - start + 2);
	return (str);
}
