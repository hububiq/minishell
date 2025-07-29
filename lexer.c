/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 21:11:25 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/23 11:08:33 by mdziadko         ###   ########.fr       */
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
		- Replace $VAR and $?, unless inside single quotes or inside 'EOF' heredocs.

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

int	isspecial(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	isquote(char c)
{
	if (c == DQUOTE || c == SQUOTE)
		return (c);
	return (0);
}

char	*extract_word(t_lexer *lx)
{
	char	*str;
	int		start;

	start = lx->i;
	while (lx->prompt[lx->i]
		&& !ft_isspace(lx->prompt[lx->i])
		&& !isspecial(lx->prompt[lx->i])
		&& !isquote(lx->prompt[lx->i]))
		lx->i++;
	str = ft_strndup(&lx->prompt[start], lx->i - start);
	if (!str)
		return (NULL);
	return (str);
}

char	*extract_quotes(t_lexer *lx)
{
	int		start;
	char	quote;
	char	*str;

	if (!isquote(lx->prompt[lx->i]))
		return (NULL);
	start = lx->i + 1;
	quote = lx->prompt[lx->i];
	lx->i++;
	lx->expand = (quote == DQUOTE);
	while (lx->prompt[lx->i] && lx->prompt[lx->i] != quote)
		lx->i++;
	if (!lx->prompt[lx->i])
	{
		lx->mini->exit_code = report_error("unclosed quotes", 258);
		return (NULL);
	}
	str = ft_strndup(&lx->prompt[start], lx->i - start);
	lx->i++;
	if (!str)
		return (NULL);
	return (str);
}

char	*extract_delim(t_lexer *lx)
{
	char	*result;
	char	*fragment;
	char	*temp;

	result = ft_strdup("");
	while (lx->prompt[lx->i] && !ft_isspace(lx->prompt[lx->i])
		&& !isspecial(lx->prompt[lx->i]))
	{
		if (isquote(lx->prompt[lx->i]))
		{
			fragment = extract_quotes(lx);
			lx->expand = false;
		}
		else
			fragment = extract_word(lx);
		if (!fragment)
			return (free(result), NULL);
		temp = result;
		result = ft_strjoin(temp, fragment);
		free(temp);
		free(fragment);
		if (!result)
			return (NULL);
	}
	return (result);
}

int	skip_whitespace(t_lexer *lx)
{
	int	saw_space;

	if (!lx || !lx->prompt)
		return (1);
	saw_space = 0;
	while (ft_isspace(lx->prompt[lx->i]))
	{
		saw_space = 1;
		lx->i++;
	}
	lx->space = saw_space;
	return (0);
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
	if (isquote(lx->prompt[lx->i]))
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

// int	tokenize_input(t_data *mini)
// {
// 	t_lexer	lx;
// 	t_token	*new;

// 	if (!mini->prompt || !*(mini->prompt))
// 		return (1);
// 	ft_bzero(&lx, sizeof(t_lexer));
// 	lx.mini = mini;
// 	lx.prompt = mini->prompt;
// 	while (lx.prompt[lx.i])
// 	{
// 		lx.expand = true;
// 		skip_whitespace(&lx);
// 		if (!lx.prompt[lx.i])
// 			break ;
// 		new = get_next_token(&lx);
// 		if (!new)
// 			return (1);
// 		if (increment_word_id(&lx, new))
// 			return (1);
// 		if (!mini->tokens)
// 			mini->tokens = new;
// 		else
// 			lx.tail->next = new;
// 		lx.tail = new;
// 	}
// 	return (0);
// }

void	print_token(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp)
	{
		if (!temp)
		{
			printf("Empty token");
			return ;
		}
		printf("%s | %i | %i | %i\n",
			temp->str, temp->type, temp->expand, temp->word_id);
		temp = temp->next;
	}
}

// int main()
// {
// 	char	*mini->prompt;
// 	t_token	*tokens;

// 	while (1)
// 	{
// 		mini->prompt = readline("Enter mini->prompt: ");
// 		printf ("%s\n", mini->prompt);
// 		tokens = tokenize_input(mini->prompt);
// 		print_token(tokens);
// 	}

// }

/* WORKING OPTION

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
	token->next = NULL;
	return (token);
}

void	print_token(t_token *token)
{
	if (!token)
	{
		printf("Empty token");
		return ;
	}
	printf("%s | %i | %i\n", token->str, token->type, token->expand);
}

int	isspecial(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	isquote(char c)
{
	if (c == DQUOTE || c == SQUOTE)
		return (c);
	return (0);
}

char	*extract_word(char *mini->prompt, int *i)
{
	int		j;
	char	*str;

	j = 0;
	while (mini->prompt[j] && !ft_isspace(mini->prompt[j])
		&& !isspecial(mini->prompt[j]) && !isquote(mini->prompt[j]))
		j++;
	str = ft_strndup(mini->prompt, j);
	if (!str)
		return (NULL);
	*i += j;
	return (str);
}

char	*extract_quotes(char *mini->prompt, int *i, bool *expand)
{
	int		j;
	char	quote;
	char	*str;

	if (!isquote(mini->prompt[0]))
		return (NULL);
	quote = mini->prompt[0];
	if (expand)
		*expand = (quote == DQUOTE);
	j = 1;
	while (mini->prompt[j] && mini->prompt[j] != quote)
		j++;
	if (mini->prompt[j] != quote)
		return (perror("No closing quotes"), NULL);
	str = ft_strndup(&mini->prompt[1], j - 1);
	if (!str)
		return (NULL);
	*i += j + 1;
	return (str);
}

t_token	*extract_delim(char *mini->prompt, int *i)
{
	int		j;
	char	*result;
	char	*fragment;
	char	*temp;
	bool	expand;

	j = 0;
	result = ft_strdup("");
	expand = !(ft_strchr(mini->prompt, SQUOTE) || ft_strchr(mini->prompt, DQUOTE));
	while (mini->prompt[j] && !ft_isspace(mini->prompt[j]) && !isspecial(mini->prompt[j]))
	{
		if (isquote(mini->prompt[j]))
			fragment = extract_quotes(&mini->prompt[j], &j, NULL);
		else
			fragment = extract_word(&mini->prompt[j], &j);
		if (!fragment)
			return (free(result), NULL);
		temp = result;
		result = ft_strjoin(temp, fragment);
		free(temp);
		free(fragment);
	}
	*i += j;
	return (add_token(result, T_WORD, expand));
}

t_token *extract_special(char *mini->prompt, int *i)
{
	if (mini->prompt[0] == '|' && (*i)++)
		return (add_token(ft_strdup("|"), T_PIPE, false));
	if (mini->prompt[0] == '>' && mini->prompt[1] == '>')
	{
		*i += 2;
		return (add_token(ft_strdup(">>"), T_REDIR_APPEND, false));
	}
	if (mini->prompt[0] == '>' && (*i)++)
		return (add_token(ft_strdup(">"), T_REDIR_OUT, false));
	if (mini->prompt[0] == '<' && mini->prompt[1] == '<')
	{
		*i += 2;
		return (add_token(ft_strdup("<<"), T_HEREDOC, false));
	}
	if (mini->prompt[0] == '<' && (*i)++)
		return (add_token(ft_strdup("<"), T_REDIR_IN, false));
	return (NULL);
}

t_token	*separate_tokens(char *mini->prompt)
{
	t_token *head;
	t_token	*new;
	t_token	*tail;
	int		i;
	bool	expand;

	head = NULL;
	tail = NULL;
	expand = true;
	i = 0;
	while (mini->prompt[i])
	{
		if (ft_isspace(mini->prompt[i]))
		{
			i++;
			continue;
		}
		if (tail && tail->type == T_HEREDOC)
			new = extract_delim(&mini->prompt[i], &i);
		else if (isspecial(mini->prompt[i]))
			new = extract_special(&mini->prompt[i], &i);
		else if (isquote(mini->prompt[i]))
			new = add_token(extract_quotes(&mini->prompt[i], &i, &expand), T_WORD, expand);
		else
			new = add_token(extract_word(&mini->prompt[i], &i), T_WORD, true);
		if (!new)
			break;
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	return (head);
}

int main()
{
	char	*mini->prompt;
	t_token	*tokens;
	t_token	*temp;

	mini->prompt = readline("Enter mini->prompt: ");
	tokens = separate_tokens(mini->prompt);
	temp = tokens;
	while(temp)
	{
		print_token(temp);
		temp = temp->next;
	}
}
*/