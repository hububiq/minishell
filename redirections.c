/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 14:47:29 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/16 01:48:25 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_close(int *fd)
{
	if (*fd == STDOUT_FILENO || *fd == STDIN_FILENO)
		return ;
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

int	open_file(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == T_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == T_REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == T_REDIR_IN || redir->type == T_HEREDOC)
		fd = open(redir->file, O_RDONLY);
	if (fd == -1)
		report_error(redir->file, errno);
	return (fd);
}

int	handle_cmd_redirs(t_parser *pars)
{
	while (pars->cur_redir)
	{
		if (pars->cur_redir->type == T_REDIR_OUT
			|| pars->cur_redir->type == T_REDIR_APPEND)
		{
			safe_close(&pars->cur_cmd->fd_out);
			pars->cur_cmd->fd_out = open_file(pars->cur_redir);
			if (pars->cur_cmd->fd_out == -1)
				return (1);
		}
		else if (pars->cur_redir->type == T_REDIR_IN
			|| pars->cur_redir->type == T_HEREDOC)
		{
			safe_close(&pars->cur_cmd->fd_in);
			pars->cur_cmd->fd_in = open_file(pars->cur_redir);
			if (pars->cur_cmd->fd_in == -1)
				return (1);
		}
		pars->cur_redir = pars->cur_redir->next;
	}
	return (0);
}

int	process_redirs(t_parser *pars)
{
	pars->cur_cmd = pars->mini->cmds;
	while (pars->cur_cmd)
	{
		pars->cur_redir = pars->cur_cmd->redirs;
		if (handle_cmd_redirs(pars))
			return (1);
		pars->cur_cmd = pars->cur_cmd->next;
	}
	return (0);
}
