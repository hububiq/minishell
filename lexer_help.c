/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:27:14 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/04 17:35:58 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspecial(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	isquote(char c)
{
	if (c == DQUOTE || c == SQUOTE)
		return (c);
	return (0);
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
