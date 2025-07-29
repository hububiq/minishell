/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:57:43 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/20 16:07:00 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error(char *msg)
{
	write(2, "mini: syntax error near unexpected token `", 43);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (258);
}

int	cmd_error(char *msg)
{
	write(2, "mini: command not found: ", 26);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (127);
}

int	report_error(char *msg, int code)
{
	if (code == 258)
		syntax_error(msg);
	else if (code == EFAULT || code == ENOENT)
		code = cmd_error(msg);
	else if (code == EACCES)
	{
		perror(msg);
		code = 126;
	}
	else
		perror(msg);
	return (code);
}

int	validate_syntax(t_data *mini)
{
	t_token	*token;

	token = mini->tokens;
	while (token)
	{
		if (token->type == T_PIPE
			&& (token->word_id == 0
				|| !token->next
				|| token->next->type == T_PIPE))
			mini->exit_code = report_error("|", 258);
		else if (token->type >= T_REDIR_IN && token->type <= T_HEREDOC)
		{
			if (!token->next)
				mini->exit_code = report_error("newline", 258);
			else if (token->next->type != T_WORD)
				mini->exit_code = report_error(token->next->str, 258);
		}
		if (mini->exit_code)
			return (mini->exit_code);
		token = token->next;
	}
	return (0);
}
