/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:21:43 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/11 13:38:48 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*init_data(char **envp)
{
	t_data	*mini;

	if (!envp || !envp[0])
		return (NULL);
	mini = malloc(sizeof(t_data));
	if (!mini)
		return (NULL);
	ft_bzero(mini, sizeof(t_data));
	mini->env = init_env(envp);
	if (!mini->env)
		return (free(mini), NULL);
	mini->prompt = NULL;
	mini->cmds = NULL;
	mini->tokens = NULL;
	mini->exit_code = 0;
	mini->last_exit_code = 0;
	mini->mini_pid = 0;
	return (mini);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)	argc;
// 	(void) 	argv;
// 	t_data	*mini = init_data(envp);
// 	print_env(mini->env, NULL);
// 	printf("\n\n");
// 	print_env(mini->env, "USER");
// 	printf("\n\n");
// 	print_env(mini->env, "COMMAND_MODE");
// }