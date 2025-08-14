/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:49:33 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/12 15:03:49 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			if (!token->next)
				return (-1);
			token = token->next;
			cur_id = token->word_id;
		}
		token = token->next;
	}
	return (count);
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
	path = find_env(pars->mini->env, "PATH");
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
