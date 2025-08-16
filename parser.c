/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:09:12 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/16 01:39:41 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_pars(t_data *mini, t_parser *pars)
{
	if (!mini->tokens)
		return (1);
	ft_bzero(pars, sizeof(t_parser));
	pars->mini = mini;
	pars->cur_token = mini->tokens;
	pars->cur_cmd = add_cmd(pars);
	return (pars->cur_cmd == NULL);
}

int	parse_cmds(t_data *mini)
{
	t_parser	pars;

	if (init_pars(mini, &pars))
		return (1);
	while (pars.cur_token)
	{
		if (pars.cur_token->type == T_PIPE)
		{
			if (handle_pipe(&pars))
				return (1);
		}
		else
		{
			if (handle_cmd(&pars))
				return (1);
		}
	}
	if (process_heredoc(&pars))
		return (1);
	if (process_redirs(&pars))
		mini->last_exit_code = 1;
	return (0);
}
