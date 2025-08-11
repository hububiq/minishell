/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:35:14 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/10 23:07:09 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(t_lexer *lx)
{
	char	*str;
	int		start;

	start = lx->i;
	while (lx->prompt[lx->i]
		&& !ft_isspace(lx->prompt[lx->i])
		&& !is_special(lx->prompt[lx->i])
		&& !is_quote(lx->prompt[lx->i]))
		lx->i++;
	str = ft_strndup(&lx->prompt[start], lx->i - start);
	if (!str)
		return (NULL);
	return (str);
}

char	*extract_quotes(t_lexer *lx)
{
	int		start;
	char	quote;
	char	*str;

	if (!is_quote(lx->prompt[lx->i]))
		return (NULL);
	start = lx->i + 1;
	quote = lx->prompt[lx->i];
	lx->i++;
	lx->expand = (quote == DQUOTE);
	while (lx->prompt[lx->i] && lx->prompt[lx->i] != quote)
		lx->i++;
	if (!lx->prompt[lx->i])
	{
		lx->mini->exit_code = report_error("unclosed quotes", 258);
		return (NULL);
	}
	str = ft_strndup(&lx->prompt[start], lx->i - start);
	lx->i++;
	if (!str)
		return (NULL);
	return (str);
}

char	*extract_delim(t_lexer *lx)
{
	char	*result;
	char	*fragment;
	char	*temp;

	result = ft_strdup("");
	while (lx->prompt[lx->i] && !ft_isspace(lx->prompt[lx->i])
		&& !is_special(lx->prompt[lx->i]))
	{
		if (is_quote(lx->prompt[lx->i]))
		{
			fragment = extract_quotes(lx);
			lx->expand = false;
		}
		else
			fragment = extract_word(lx);
		if (!fragment)
			return (free(result), NULL);
		temp = result;
		result = ft_strjoin(temp, fragment);
		free(temp);
		free(fragment);
		if (!result)
			return (NULL);
	}
	return (result);
}
