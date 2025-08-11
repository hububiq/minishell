/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 21:11:25 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/10 23:07:09 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	1. Lexing, devide into tokens:
		- words, 
		- operators, 
		- quoted strings, 
		- redirections
	
	2. Quote Resolution
		- ' disables all expansions
		- " allows $, but disables other expansions
		- Ignore \ (backslash)
		- Ignore (or error) ; (semicolon) 
		- Error for unclosed quotes

	3. Variable Expansion
		- Replace $VAR and $?, unless inside single
		quotes or inside 'EOF' heredocs.

	4. Command Splitting
		- Based on pipes (|) → creates list of command units.

	5. Redirection Resolution (per command unit):
		- Process all redirections (<, >, >>, <<) left to right.
		- Last redirection of same type wins.
		- Redirection has higher precedence than piping.

	6. Here-document Processing. For each heredoc << EOF, shell:
		- Reads from stdin until EOF line is found.
		- Saves that content into a temporary file (or pipe it).
		- Sets that file as the closest command’s input (stdin).

	7. I/O Setup. For each command:
		- Apply the resolved redirections.
		- Setup pipes if needed.
	
	8. Execute Commands
		- If built-in: execute in current shell.
		- Else: fork + execve.
	
	9. Handle exit code
		- Store pid for each process
		- Wait all process end and return the last status

	10. Reset signal handlers and terminal settings
*/

int	init_lexer(t_data *mini, t_lexer *lx)
{
	if (!mini->prompt || !*(mini->prompt))
		return (1);
	ft_bzero(lx, sizeof(t_lexer));
	lx->mini = mini;
	lx->prompt = mini->prompt;
	lx->expand = true;
	return (0);
}

t_token	*add_token(char *str, t_type type, bool expand)
{
	t_token	*token;

	if (!str)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = str;
	token->type = type;
	token->expand = expand;
	token->word_id = 0;
	token->next = NULL;
	return (token);
}

t_token	*get_next_token(t_lexer *lx)
{
	if (!lx->prompt[lx->i])
		return (NULL);
	if (lx->prompt[lx->i] == '<'
		&& lx->prompt[lx->i + 1] == '<')
		return (lx->i += 2, add_token(ft_strdup("<<"), T_HEREDOC, false));
	if (lx->prompt[lx->i] == '>'
		&& lx->prompt[lx->i + 1] == '>')
		return (lx->i += 2, add_token(ft_strdup(">>"), T_REDIR_APPEND, false));
	if (lx->prompt[lx->i] == '|')
		return (lx->i++, add_token(ft_strdup("|"), T_PIPE, false));
	if (lx->prompt[lx->i] == '>')
		return (lx->i++, add_token(ft_strdup(">"), T_REDIR_OUT, false));
	if (lx->prompt[lx->i] == '<')
		return (lx->i++, add_token(ft_strdup("<"), T_REDIR_IN, false));
	if (lx->tail && lx->tail->type == T_HEREDOC)
		return (add_token(extract_delim(lx), T_WORD, lx->expand));
	if (is_quote(lx->prompt[lx->i]))
		return (add_token(extract_quotes(lx), T_WORD, lx->expand));
	return (add_token(extract_word(lx), T_WORD, lx->expand));
}

int	increment_word_id(t_lexer *lx, t_token *token)
{
	int	increment;

	if (!lx || !token)
		return (1);
	if (!lx->tail)
	{
		token->word_id = 0;
		return (0);
	}
	if (lx->space)
		increment = 1;
	else if (lx->tail->type != token->type)
		increment = 1;
	else
		increment = 0;
	token->word_id = lx->tail->word_id + increment;
	return (0);
}

int	tokenize_input(t_data *mini)
{
	t_lexer	lx;
	t_token	*new;

	if (init_lexer(mini, &lx))
		return (1);
	while (lx.prompt[lx.i])
	{
		lx.expand = true;
		skip_whitespace(&lx);
		if (!lx.prompt[lx.i])
			break ;
		new = get_next_token(&lx);
		if (!new)
			return (1);
		if (increment_word_id(&lx, new))
			return (1);
		if (!mini->tokens)
			mini->tokens = new;
		else
			lx.tail->next = new;
		lx.tail = new;
	}
	return (0);
}
