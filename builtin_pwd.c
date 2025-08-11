/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:48:33 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/11 13:36:44 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_cmd *cmd)
{
	char	cwd[4096];

	(void)cmd;
	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
	return (0);
}

/*


int	builtin_pwd(t_data *mini, char **args)
{
	if (!mini || !args || !*args || ft_strcmp("pwd", args[0]))
		return (1);
	print_env(mini->env, "PWD");
	return (0);
}

int	builtin_pwd(t_cmd *cmd)
{
	char	cwd[4096];

	(void)cmd;
	getcwd(cwd, sizeof(cwd));
	// if (cwd)
	// {
		printf("%s\n", cwd);
		return (0);
	//}
	// else
	// {
	// 	perror("minishell: pwd");
	// 	return (1);
	// }
}


*/