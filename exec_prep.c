/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_prep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 14:47:29 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/27 15:21:35 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redirs(t_parser *pars)
{
	pars->cur_cmd = pars->mini->cmds;
	while (pars->cur_cmd)
	{
		// if (handle_fd(pars))
		// 	return (1);
		pars->cur_cmd = pars->cur_cmd->next;
	}
	return (0);
}
