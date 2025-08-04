/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:14:39 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/03 18:47:43 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_prompt(t_data *mini)
{
	printf("------TOKENIZATION------\n");
	if (tokenize_input(mini))
		return (free_data(mini), mini->exit_code);
	print_token(mini->tokens);
	if (validate_syntax(mini))
		return (free_data(mini), mini->exit_code);
	printf("------EXPANSION------\n");
	if (expand_tokens(mini))
		return (free_data(mini), mini->exit_code);
	print_token(mini->tokens);
	printf("------PARSING------\n");
	if (parse_cmds(mini))
		return (free_data(mini), mini->exit_code);
	print_cmd(mini);
	return (free_data(mini), 0);
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
