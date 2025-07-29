/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:14:39 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/28 22:53:42 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// /* At top of main.c, before main() */
// static void	sigint_handler(int sig)
// {
//     (void)sig;
//     write(1, "\n", 1);
//     rl_on_new_line();
//     rl_replace_line("", 0);
//     rl_redisplay();
// }

// int	main(int argc, char **argv, char **envp)
// {
//     t_data	*mini;

//     (void)argc; (void)argv;
//     mini = init_data(envp);
//     if (!mini)
//         return (perror("Init data error"), EXIT_FAILURE);

//     /* --- install handlers --- */
//     signal(SIGINT, sigint_handler);
//     signal(SIGQUIT, SIG_IGN);

//     while (1)
//     {
//         mini->prompt = readline("mini> ");
//         if (!mini->prompt)  /* Ctrl-D */
//         {
//             write(1, "exit\n", 5);
//             free_data(mini);
//             free_env_list(mini->env);
//             rl_clear_history();
//             free(mini);
//             exit(mini->last_exit_code);
//         }
//         add_history(mini->prompt);
//         mini->last_exit_code = run_prompt(mini);
//         mini->exit_code      = 0;
//     }
//     return (0);
// }

// int	run_prompt(t_data *mini)
// {
// 	printf("------TOKENIZATION------\n");
// 	if (tokenize_input(mini))
// 	{
// 		if (mini->exit_code)
// 			return (free_data(mini), mini->exit_code);
// 		return (perror("mini: tokenization error"), free_data(mini), 1);
// 	}
// 	print_token(mini->tokens);
// 	if (validate_syntax(mini))
// 	{
// 		if (mini->exit_code)
// 			return (free_data(mini), mini->exit_code);
// 		return (perror("mini: syntax error"), free_data(mini), 1);
// 	}
// 	print_token(mini->tokens);
// 	printf("------EXPANTION------\n");
// 	if (expand_tokens(mini))
// 	{
// 		if (mini->exit_code)
// 			return (free_data(mini), mini->exit_code);
// 		return (perror("mini: expansion error"), free_data(mini), 1);
// 	}
// 	print_token(mini->tokens);
// 	printf("------PARSING------\n");
// 	if (parse_cmds(mini))
// 		return (perror("mini: parsing error"), free_data(mini), 1);
// 	print_cmd(mini);
// 	return (free_data(mini), 0);
// }


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

int	main(int argc, char **argv, char **envp)
{
	t_data	*mini;

	(void)argc;
	(void)argv;
	mini = init_data(envp);
	if (!mini)
		return (perror("Init data error\n"), EXIT_FAILURE);
	setup_signals();
	while (1)
	{
		mini->prompt = readline(GREEN"mini> "RESET);
		if (!mini->prompt)
		{
			printf("\nexit\n");
			return (free_program(mini), 0);
		}
		if (!mini->prompt[0])
		{
			free(mini->prompt);
			continue ;
		}
		if (*mini->prompt)
			add_history(mini->prompt);
		mini->last_exit_code = run_prompt(mini);
		mini->exit_code = 0;
	}
}
