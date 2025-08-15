/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:52:41 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/15 20:08:44 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_program(t_data *mini)
{
	if (!mini)
		return ;
	free_env_list(mini->env);
	mini->env = NULL;
	free_data(mini);
	free(mini);
	rl_clear_history();
}

void	free_data(t_data *mini)
{
	if (!mini)
		return ;
	free_token_list(mini->tokens);
	mini->tokens = NULL;
	unlink_tmp(mini);
	free_cmd_list(mini->cmds);
	mini->cmds = NULL;
	free(mini->prompt);
	mini->prompt = NULL;
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
			safe_close(&cmd->fd_in);
		if (cmd->fd_out != STDOUT_FILENO)
			safe_close(&cmd->fd_out);
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
