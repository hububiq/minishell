/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 10:58:01 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/11 13:59:18 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	EXIT [status]

	Exit the shell.
	exit alone → use last exit code.
	exit 42 → set status 42.
	Too many args: error and don’t exit.
	Non-numeric: error and exit with 255.
*/

bool	is_number(char	*str)
{
	if (!str || !*str)
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	count_args(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd->args[count])
		count++;
	return (count);
}

int	builtin_exit(t_cmd *cmd, t_data *mini)
{
	int	exit_code;

	if (!mini || !cmd || !cmd->args || !cmd->args[0])
		return (1);
	printf("exit\n");
	if (cmd->args[1] && !is_number(cmd->args[1]))
	{
		report_error("exit: numeric argument required", 1);
		free_program(mini);
		exit(255);
	}
	else if (count_args(cmd) > 2)
	{
		report_error("exit: too many arguments", 1);
		return (1);
	}
	exit_code = mini->last_exit_code;
	if (cmd->args[1])
		exit_code = ft_atoi(cmd->args[1]) % 256;
	free_program(mini);
	exit(exit_code);
}

// int	builtin_exit(t_cmd *cmd, t_data *mini)
// {
// 	if (!mini || !cmd || !cmd->args || !cmd->args[0])
// 		return (1);
// 	printf("exit\n");
// 	if (cmd->args[1] && !is_number(cmd->args[1]))
// 	{
// 		report_error("exit: numeric argument required", 1);
// 		free_program(mini);
// 		exit(255);
// 	}
// 	else if (cmd->args[2])
// 	{
// 		report_error("exit: too many arguments", 1);
// 		return (1);
// 	}
// 	free_program(mini);
// 	if (cmd->args[1])
// 		exit(ft_atoi(cmd->args[1]) % 256);
// 	exit(mini->last_exit_code);
// }
