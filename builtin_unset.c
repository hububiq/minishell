/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:58:04 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/11 13:25:59 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	UNSET KEY
	Remove environment variable.
*/

int	builtin_unset(t_cmd *cmd, t_data *mini)
{
	int	i;

	if (!mini || !cmd || !cmd->args || !cmd->args[0])
		return (1);
	i = 1;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '=') || ft_isdigit(cmd->args[i][0]))
		{
			report_error("unset: not a valid identifier", 1);
			i++;
			continue ;
		}
		delete_env(&mini->env, cmd->args[i]);
		i++;
	}
	return (0);
}
