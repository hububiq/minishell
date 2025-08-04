/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:04:34 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/04 18:04:45 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_child(t_data *mini, int child_pid)
{
	int	status;

	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		mini->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->exit_code = 128 + WTERMSIG(status);
	else
		return (1);
	return (mini->exit_code != 0);
}
