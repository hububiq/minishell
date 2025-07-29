/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 14:42:31 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/29 18:17:23 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// in main
// ->setup processing SIGINT and SIGQUIT
// in heredoc
// -> temporarily set SIG_IGN for SIGINT and SIGQUIT 
// -> save old_sa
// fork()
// -> in child set SIGINT and SIGQUIT to SIG_DFL
// ->in parent restore old_sa and wait status from child.

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	sigint_handler(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\r\n", 2);
	//rl_replace_line("", 0); // UNCOMMENT IT ON LINUX
	rl_on_new_line();
	rl_redisplay();
}

void	ignore_signals(struct sigaction *old_sa)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old_sa);
	sigaction(SIGQUIT, &sa, old_sa);
}

void	set_default_signals(int signo)
{
	(void)signo;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	restore_old_signals(struct sigaction *old_sa)
{
	sigaction(SIGINT, old_sa, NULL);
	sigaction(SIGQUIT, old_sa, NULL);
}
