/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:40:08 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/11 13:41:09 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *str)
{
	return (!ft_strcmp(str, "echo")
		|| !ft_strcmp(str, "cd")
		|| !ft_strcmp(str, "pwd")
		|| !ft_strcmp(str, "export")
		|| !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "env")
		|| !ft_strcmp(str, "exit"));
}

void	restore_fds(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

bool	is_parent_mod_builtins(char *cmd_name)
{
	if (!cmd_name)
		return (false);
	if (ft_strcmp(cmd_name, "cd") == 0
		|| ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0
		|| ft_strcmp(cmd_name, "exit") == 0)
		return (true);
	return (false);
}

int	execute_builtin(t_cmd *cmd, t_data *mini)
{
	int	exit_status;

	exit_status = 0;
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		exit_status = builtin_echo(cmd);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		exit_status = builtin_pwd(cmd);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		exit_status = builtin_cd(cmd, mini);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		exit_status = builtin_export(cmd, mini);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		exit_status = builtin_unset(cmd, mini);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		exit_status = builtin_env(cmd, mini);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		exit_status = builtin_exit(cmd, mini);
	mini->last_exit_code = exit_status;
	return (exit_status);
}
