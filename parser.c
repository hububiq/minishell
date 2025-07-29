/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:09:12 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/29 10:33:27 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static void	print_pars(t_parser *pars)
{
	printf("\n-----PRINT PARSER-----\n");
	printf("cur_cmd : %p\n", pars->cur_cmd);
	printf("arg %i : %s\n", pars->arg_i, pars->cur_cmd->args[pars->arg_i]);
	printf("cur_token : %s (word_id = %i)\n",
		pars->cur_token->str, pars->cur_token->word_id);
	printf("cur_redir : %p\n", pars->cur_redir);
	if (pars->cur_redir && pars->cur_redir->type == T_HEREDOC)
		printf("delim : %s\n", pars->cur_redir->delim);
	else if (pars->cur_redir)
		printf("file : %s\n", pars->cur_redir->file);
}
*/

bool	is_builtin(char *str)
{
	return (!ft_strcmp(str, "echo")
		|| !ft_strcmp(str, "cd")
		|| !ft_strcmp(str, "pwd")
		|| !ft_strcmp(str, "export")
		|| !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "env")
		|| !ft_strcmp(str, "exit"));
}

int	find_executable(t_cmd *cmd, char **dirs)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	if (!dirs)
		return (0);
	while (dirs[i])
	{
		temp = ft_strjoin(dirs[i], "/");
		if (!temp)
			return (0);
		full_path = ft_strjoin(temp, cmd->args[0]);
		free (temp);
		if (!full_path)
			return (0);
		if (access(full_path, X_OK) == 0)
		{
			cmd->path = full_path;
			return (1);
		}
		free(full_path);
		i++;
	}
	return (0);
}

char	*find_cmd_path(t_parser *pars)
{
	t_env	*path;
	char	**dirs;
	char	*result;

	result = NULL;
	if (ft_strchr(pars->cur_cmd->args[0], '/'))
	{
		if (access(pars->cur_cmd->args[0], X_OK) == 0)
			return (ft_strdup(pars->cur_cmd->args[0]));
		return (NULL);
	}
	path = find_env(pars->mini, "PATH");
	if (!path || !path->value)
		return (NULL);
	dirs = ft_split(path->value, ':');
	if (!dirs)
		return (NULL);
	if (find_executable(pars->cur_cmd, dirs))
		result = pars->cur_cmd->path;
	free_split(dirs);
	return (result);
}

int	count_cmd_args(t_token *token)
{
	int	count;
	int	cur_id;

	count = 0;
	cur_id = -1;
	while (token && token->type != T_PIPE)
	{
		if (token->type == T_WORD && token->word_id != cur_id)
		{
			cur_id = token->word_id;
			count++;
		}
		else if (token->type >= T_REDIR_IN && token->type <= T_HEREDOC)
		{
			if(!token->next)
				return (1);
			token = token->next;
			cur_id = token->word_id;
		}
		token = token->next;
	}
	printf ("COUNT ARGS: %i\n", count);
	return (count);
}

t_cmd	*add_cmd(t_parser *pars)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	ft_bzero(new_cmd, sizeof(t_cmd));
	new_cmd->fd_in = STDIN_FILENO;
	new_cmd->fd_out = STDOUT_FILENO;
	new_cmd->next = NULL;
	pars->arg_count = count_cmd_args(pars->cur_token);
	new_cmd->args = ft_calloc(pars->arg_count + 1, sizeof(char *));
	if (!new_cmd->args)
		return (NULL);
	if (!pars->mini->cmds)
		pars->mini->cmds = new_cmd;
	else if (pars->cur_cmd)
		pars->cur_cmd->next = new_cmd;
	else
		return (NULL);
	pars->cur_redir = NULL;
	pars->arg_i = 0;
	return (new_cmd);
}

char	*join_tokens_by_id(t_token **token)
{
	char	*result;
	char	*temp;

	if (!token || !*token)
		return (NULL);
	if (!(*token)->next || (*token)->word_id != (*token)->next->word_id)
		return (ft_strdup((*token)->str));
	result = ft_strdup((*token)->str);
	while (*token && (*token)->next
		&& (*token)->word_id == (*token)->next->word_id)
	{
		temp = result;
		result = ft_strjoin(temp, (*token)->next->str);
		free(temp);
		if (!result)
			return (NULL);
		*token = (*token)->next;
	}
	return (result);
}

int	add_arg(t_parser *pars)
{
	char	*arg;

	if (!pars->cur_token || !pars->cur_cmd || !pars->cur_cmd->args)
		return (1);
	if (pars->arg_i >= pars->arg_count)
		return (1);
	arg = join_tokens_by_id(&pars->cur_token);
	if (!arg)
		return (1);
	pars->cur_cmd->args[pars->arg_i] = arg;
	if (pars->arg_i == 0)
	{
		pars->cur_cmd->builtin = is_builtin(arg);
		if (!pars->cur_cmd->builtin)
			pars->cur_cmd->path = find_cmd_path(pars);
	}
	pars->arg_i++;
	return (0);
}

int	add_redir(t_parser *pars, t_type type)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (1);
	ft_bzero(new_redir, sizeof(t_redir));
	new_redir->type = type;
	pars->cur_token = pars->cur_token->next;
	if (type == T_HEREDOC)
	{
		new_redir->expand = pars->cur_token->expand;
		new_redir->delim = join_tokens_by_id(&pars->cur_token);
	}
	else
		new_redir->file = join_tokens_by_id(&pars->cur_token);
	new_redir->next = NULL;
	if (!pars->cur_cmd->redirs)
		pars->cur_cmd->redirs = new_redir;
	else
		pars->cur_redir->next = new_redir;
	pars->cur_redir = new_redir;
	return (0);
}

int	init_pars(t_data *mini, t_parser *pars)
{
	if (!mini->tokens)
		return (1);
	ft_bzero(pars, sizeof(t_parser));
	pars->mini = mini;
	pars->cur_token = mini->tokens;
	pars->cur_cmd = add_cmd(pars);
	return (pars->cur_cmd == NULL);
}

int	handle_pipe(t_parser *pars)
{
	pars->cur_token = pars->cur_token->next;
	pars->cur_cmd = add_cmd(pars);
	return (pars->cur_cmd == NULL);
}

int	handle_cmd(t_parser *pars)
{
	if (pars->cur_token->type == T_WORD)
	{
		if (add_arg(pars))
			return (1);
	}
	else if (pars->cur_token->type >= T_REDIR_IN
		&& pars->cur_token->type <= T_HEREDOC)
	{
		if (add_redir(pars, pars->cur_token->type))
			return (1);
	}
	else
		return (1);
	pars->cur_token = pars->cur_token->next;
	return (0);
}

int	parse_cmds(t_data *mini)
{
	t_parser	pars;

	if (init_pars(mini, &pars))
		return (1);
	while (pars.cur_token)
	{
		if (pars.cur_token->type == T_PIPE)
		{
			if (handle_pipe(&pars))
				return (1);
		}
		else
		{
			if (handle_cmd(&pars))
				return (1);
		}
	}
	if (process_heredoc(&pars))
		return (1);
	return (0);
}

void	print_cmd(t_data *mini)
{
	int		cmd_i;
	t_cmd	*cur_cmd;
	t_redir	*cur_redir;

	if (!mini->cmds)
		return ;
	cmd_i = 0;
	cur_cmd = mini->cmds;
	printf("\n-----PRINT COMMAND-----\n");
	while (cur_cmd)
	{
		printf("cmd %i : ", cmd_i);
		if (!cur_cmd->args || !cur_cmd->args[0])
			printf("(null)\n");
		else if (cur_cmd->args)
		{
			for (int i = 0; cur_cmd->args[i]; i++)
				printf("arg %d: %s\n", i + 1, cur_cmd->args[i]);
		}
		printf("path      : %s\n", cur_cmd->path);
		printf("is builtin: %i\n", cur_cmd->builtin);
		printf("fd_in     : %i\n", cur_cmd->fd_in);
		printf("fd_out    : %i\n", cur_cmd->fd_out);

		cur_redir = cur_cmd->redirs;
		if (!cur_redir)
			printf("redirs    : (none)\n");
		else
		{
			printf("redirs:\n");
			while (cur_redir)
			{
				printf("  type : %d | ", cur_redir->type);
				printf("delim : %s\n", cur_redir->delim);
				printf("file  : %s\n", cur_redir->file);
				if (cur_redir->type == T_HEREDOC)
				{
					int fd = open(cur_redir->file, O_RDONLY, 0644);
					char output[51];
					int bytes_read;
					while ((bytes_read = read(fd, output, 50)) > 0)
					{
						output[bytes_read] = '\0';
						printf("%s", output);
					}
					close(fd);
				}
				printf("expand : %d | ", cur_redir->expand);
				cur_redir = cur_redir->next;
			}
		}
		printf("\n----------------------------------\n");
		cmd_i++;
		cur_cmd = cur_cmd->next;
	}
}

/* WORKING OPTION


bool	is_builtin(char *str)
{
	return (!ft_strcmp(str, "echo")
		|| !ft_strcmp(str, "cd")
		|| !ft_strcmp(str, "pwd")
		|| !ft_strcmp(str, "export")
		|| !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "env")
		|| !ft_strcmp(str, "exit"));
}

int	find_executable(t_cmd *cmd, char **dirs)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	if (!dirs)
		return (0);
	while (dirs[i])
	{
		temp = ft_strjoin(dirs[i], "/");
		if (!temp)
			return (0);
		full_path = ft_strjoin(temp, cmd->args[0]);
		free (temp);
		if (!full_path)
			return (0);
		if (access(full_path, X_OK) == 0)
		{
			cmd->path = full_path;
			return (1);
		}
		free(full_path);
		i++;
	}
	return (0);
}

char	*find_cmd_path(t_parser *pars)
{
	t_env	*path;
	char	**dirs;
	
	path = find_env(pars->mini, "PATH");
	dirs = ft_split(path->value, ':');
	if (!dirs)
		return (NULL);
	if (!find_executable(pars->cur_cmd, dirs))
	{
		free_split(dirs);
		return (NULL);
	}
	free_split(dirs);
	return (pars->cur_cmd->path);
}

int	count_cmd_args(t_token *token)
{
	int	count;
	int	cur_id;
	
	count = 0;
	cur_id = -1;
	while (token && token->type != T_PIPE)
	{
		if ((token->type == T_WORD || token->type == T_QUOTE)
			&& token->word_id != cur_id)
		{
			cur_id = token->word_id;
			count++;
		}
		else if (token->type >= T_REDIR_IN && token->type <= T_HEREDOC)
		{
			if (!token->next)
				return (258);
			token = token->next;
			cur_id = token->word_id;
		}
		token = token->next;
	}
	printf ("COUNT ARGS: %i\n", count);
	return (count);
}

t_cmd	*add_cmd(t_parser *pars)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (pars->mini->exit_code = 1, NULL);
	ft_bzero(new_cmd, sizeof(t_cmd));
	new_cmd->fd_in = STDIN_FILENO;
	new_cmd->fd_out = STDOUT_FILENO;
	new_cmd->next = NULL;
	pars->arg_count = count_cmd_args(pars->cur_token);
	new_cmd->args = ft_calloc(pars->arg_count + 1, sizeof(char *));
	if (!new_cmd->args)
		return (pars->mini->exit_code = 1, NULL);
	if (!pars->mini->cmds)
		pars->mini->cmds = new_cmd;
	else if (pars->cur_cmd)
		pars->cur_cmd->next = new_cmd;
	else
		return (pars->mini->exit_code = 1, NULL);
	pars->cur_redir = NULL;
	pars->arg_i = 0;
	return (new_cmd);
}

char	*join_tokens_by_id(t_token **token)
{
	char	*result;
	char	*temp;

	if (!token || !*token)
		return (NULL);
	if (!(*token)->next || (*token)->word_id != (*token)->next->word_id)
		return (ft_strdup((*token)->str));
	result = ft_strdup((*token)->str);
	while (*token && (*token)->next
		&& (*token)->word_id == (*token)->next->word_id)
	{
		temp = result;
		result = ft_strjoin(temp, (*token)->next->str);
		free(temp);
		if (!result)
			return (NULL);
		*token = (*token)->next;
	}
	return (result);
}

int	add_arg(t_parser *pars)
{
	char	*arg;

	if (!pars->cur_token || !pars->cur_cmd || !pars->cur_cmd->args)
		return (1);
	if (pars->arg_i >= pars->arg_count)
		return (1);
	arg = join_tokens_by_id(&pars->cur_token);
	if (!arg)
		return (1);
	pars->cur_cmd->args[pars->arg_i] = arg;
	if (pars->arg_i == 0)
	{
		pars->cur_cmd->builtin = is_builtin(arg);
		if (!pars->cur_cmd->builtin)
			pars->cur_cmd->path = find_cmd_path(pars);
	}
	pars->arg_i++;
	return (0);
}

int	add_redir(t_parser *pars, t_type type)
{
	t_redir	*new_redir;

	if (!pars->cur_token->next || !pars->cur_cmd)
		return (258);
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (1);
	ft_bzero(new_redir, sizeof(t_redir));
	new_redir->type = type;
	pars->cur_token = pars->cur_token->next;
	if (type == T_HEREDOC)
		new_redir->delim = join_tokens_by_id(&pars->cur_token);
	else
		new_redir->file = join_tokens_by_id(&pars->cur_token);
	new_redir->next = NULL;
	if (!pars->cur_cmd->redirs)
		pars->cur_cmd->redirs = new_redir;
	else
		pars->cur_redir->next = new_redir;
	pars->cur_redir = new_redir;
	return (0);
}

int	init_pars(t_data *mini, t_parser *pars)
{
	if (!mini->tokens)
		return (1);
	if (mini->tokens->type == T_PIPE)
		return (258);
	ft_bzero(pars, sizeof(t_parser));
	pars->mini = mini;
	pars->cur_token = mini->tokens;
	return (0);
}

int	parse_cmds(t_data *mini)
{
	t_parser	pars;

	mini->exit_code = init_pars(mini, &pars);
	pars.cur_cmd = add_cmd(&pars);
	while (pars.cur_token && !mini->exit_code)
	{
		if (pars.cur_token->type == T_PIPE)
		{
			if (!pars.cur_token->next)
				return (258);
			pars.cur_token = pars.cur_token->next;
			pars.cur_cmd = add_cmd(&pars);
			continue;
		}
		else if (pars.cur_token->type == T_WORD 
			|| pars.cur_token->type == T_QUOTE)
			mini->exit_code = add_arg(&pars);
		else
			mini->exit_code = add_redir(&pars, pars.cur_token->type);
		//print_pars(&pars);
		pars.cur_token = pars.cur_token->next;
	}
	return (mini->exit_code);
}

void	print_cmd(t_data *mini)
{
	int		cmd_i;
	t_cmd	*cur_cmd;
	t_redir	*cur_redir;

	if (!mini->cmds)
		return ;
	cmd_i = 0;
	cur_cmd = mini->cmds;
	printf("\n-----PRINT COMMAND-----\n");
	while (cur_cmd)
	{
		printf("cmd %i : ", cmd_i);
		if (!cur_cmd->args || !cur_cmd->args[0])
			printf("(null)\n");
		else if (cur_cmd->args)
		{
			for (int i = 0; cur_cmd->args[i]; i++)
				printf("arg %d: %s\n", i + 1, cur_cmd->args[i]);
		}
		printf("path      : %s\n", cur_cmd->path);
		printf("is builtin: %i\n", cur_cmd->builtin);
		printf("fd_in     : %i\n", cur_cmd->fd_in);
		printf("fd_out    : %i\n", cur_cmd->fd_out);

		cur_redir = cur_cmd->redirs;
		if (!cur_redir)
			printf("redirs    : (none)\n");
		else
		{
			printf("redirs:\n");
			while (cur_redir)
			{
				printf("  type : %d | ", cur_redir->type);
				if (cur_redir->type == T_HEREDOC)
					printf("delim : %s\n", cur_redir->delim);
				else
					printf("file  : %s\n", cur_redir->file);
				cur_redir = cur_redir->next;
			}
		}
		printf("----------------------------------\n");
		cmd_i++;
		cur_cmd = cur_cmd->next;
	}
}

*/