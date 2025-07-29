/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:52:41 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/28 23:04:17 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_program(t_data *mini)
{
	if (!mini)
		return ;
	free_data(mini);
	free_env_list(mini->env);
	free(mini);
	clear_history(); //rl_clear_history(); ON LINUX
}

void	free_data(t_data *mini)
{
	if (!mini)
		return ;
	free_token_list(mini->tokens);
	mini->tokens = NULL;
	free_cmd_list(mini->cmds);
	mini->cmds = NULL;
	free(mini->prompt);
	mini->prompt = NULL;
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr || !*arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_redir_list(t_redir *redir)
{
	t_redir	*temp;

	if (!redir)
		return ;
	while (redir)
	{
		temp = redir->next;
		free(redir->delim);
		free(redir->file);
		free(redir);
		redir = temp;
	}
	redir = NULL;
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*temp;

	if (!cmd)
		return ;
	while (cmd)
	{
		temp = cmd->next;
		free(cmd->path);
		free_arr(cmd->args);
		free_redir_list(cmd->redirs);
		if (cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		if (cmd->fd_out != STDOUT_FILENO)
			close(cmd->fd_out);
		free(cmd);
		cmd = temp;
	}
}

void	free_token_list(t_token	*token)
{
	t_token	*temp;

	if (!token)
		return ;
	while (token)
	{
		temp = token->next;
		free(token->str);
		free(token);
		token = temp;
	}
}

void	free_env(t_env *env)
{
	if (!env)
		return ;
	free(env->key);
	free(env->value);
	free(env);
}

void	free_env_list(t_env *env)
{
	t_env	*temp;

	if (!env)
		return ;
	while (env)
	{
		temp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = temp;
	}
}

// void	safe_close(int *fd)
// {
// 	if (*fd != -1)
// 	{
// 		close(*fd);
// 		*fd = -1;
// 	}
// }
