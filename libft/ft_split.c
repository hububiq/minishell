/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:30:30 by mdziadko          #+#    #+#             */
/*   Updated: 2025/05/09 20:05:18 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	token_count(char const *s, char c)
{
	int	i;
	int	tokens;

	i = 0;
	tokens = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			continue ;
		}
		if (i == 0 || s[i - 1] == c)
			tokens++;
		i++;
	}
	return (tokens);
}

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static	char	**token_extract(char **arr, char const *s, char c)
{
	int	i;
	int	start;
	int	end;
	int	res_i;

	i = 0;
	res_i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		end = i;
		if (start < end)
		{
			arr[res_i] = ft_substr(s, start, end - start);
			if (!arr[res_i])
				return (free_split(arr), NULL);
			res_i++;
		}
	}
	arr[res_i] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		size;

	if (!s)
		return (NULL);
	size = token_count(s, c);
	res = malloc((size + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	return (token_extract(res, s, c));
}

/* WORKING VERSION

int	token_count(char const *s, char c)
{
	int	i;
	int	tokens;

	i = 0;
	tokens = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			continue ;
		}
		if (i == 0 || s[i - 1] == c)
			tokens++;
		i++;
	}
	return (tokens);
}

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static	char	**token_extract(char **arr, char const *s, char c)
{
	int	i;
	int	start;
	int	end;
	int	res_i;

	i = 0;
	res_i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		end = i;
		if (start < end)
		{
			arr[res_i] = ft_substr(s, start, end - start);
			if (!arr[res_i])
				return (free_split(arr), NULL);
			res_i++;
		}
	}
	arr[res_i] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		size;
	char	*trim_s;

	if (!s)
		return (NULL);
	// trim_s = ft_strtrim(s, "\n");
	// trim_s = ft_strtrim(s, &c);
	
	size = token_count(s, c);
	res = malloc((size + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	return (token_extract(res, s, c));
}
*/
