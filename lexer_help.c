/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:27:14 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/17 19:12:57 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_special(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (true);
	return (false);
}

bool	is_quote(char c)
{
	if (c == DQUOTE || c == SQUOTE)
		return (true);
	return (false);
}

int	skip_whitespace(t_lexer *lx)
{
	int	saw_space;

	if (!lx || !lx->prompt)
		return (1);
	saw_space = 0;
	while (ft_isspace(lx->prompt[lx->i]))
	{
		saw_space = 1;
		lx->i++;
	}
	lx->space = saw_space;
	return (0);
}
