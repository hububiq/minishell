/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:14:39 by mdziadko          #+#    #+#             */
/*   Updated: 2025/05/18 11:09:36 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main (int argc, char **argv, char **env)
{
	char	*rl;
	
	(void) argc;
	(void) argv;
	(void) env;
	while (1)
	{
		rl = readline("mini> ");
		if (!rl)
			exit(EXIT_FAILURE);
		if (!ft_strncmp(rl, "exit", 4))
		{
			free (rl);
			exit(EXIT_SUCCESS);
		}
		printf("%s\n", rl);
		free (rl);
	}
}