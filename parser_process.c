/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:50:53 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/06 17:43:45 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	new_cmd->pid = -1;
	pars->arg_count = count_cmd_args(pars->cur_token);
	if (pars->arg_count < 0)
		return (NULL);
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
