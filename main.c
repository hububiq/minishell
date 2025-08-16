/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:14:39 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/16 11:58:40 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	run_prompt(t_data *mini)
// {
// 	if (tokenize_input(mini))
// 		return (free_data(mini), mini->exit_code);
// 	if (validate_syntax(mini))
// 		return (free_data(mini), mini->exit_code);
// 	if (expand_tokens(mini))
// 		return (free_data(mini), mini->exit_code);
// 	if (parse_cmds(mini))
// 		return (free_data(mini), mini->exit_code);
// 	if (execute(mini))
// 		return (free_data(mini), mini->exit_code);
// 	return (free_data(mini), 0);
// }

int	run_prompt(t_data *mini)
{
	int	code;

	if (tokenize_input(mini)
		|| validate_syntax(mini)
		|| expand_tokens(mini))
	{
		code = mini->last_exit_code;
		free_data(mini);
		return (code);
	}
	if (parse_cmds(mini))
		return (free_data(mini), mini->exit_code);
	execute(mini);
	code = mini->last_exit_code;
	free_data(mini);
	return (code);
}

void	run_mini(t_data	*mini)
{
	while (1)
	{
		setup_signals();
		g_sig = 0;
		mini->prompt = readline(GREEN"mini> "RESET);
		if (!mini->prompt)
		{
			printf("exit\n");
			free_program(mini);
			return ;
		}
		if (g_sig == SIGINT)
			mini->last_exit_code = 130;
		if (!mini->prompt[0])
		{
			free(mini->prompt);
			continue ;
		}
		add_history(mini->prompt);
		mini->last_exit_code = run_prompt(mini);
		mini->exit_code = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*mini;

	(void)argc;
	(void)argv;
	mini = init_data(envp);
	if (!mini)
		return (perror("Init data error\n"), EXIT_FAILURE);
	run_mini(mini);
	return (EXIT_SUCCESS);
}
