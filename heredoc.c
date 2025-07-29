/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 21:02:29 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/29 22:33:43 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_child(t_data *mini, int child_pid)
{
	int	status;

	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		mini->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->exit_code = 128 + WTERMSIG(status);
	else
		return (1);
	return (mini->exit_code != 0);
}

int	write_to_heredoc(t_parser *pars, int fd)
{
	char	*rl;
	char	*to_write;

	while (true)
	{
		rl = readline(CYAN"<< "RESET);
		if (!rl)
			break ;
		if (!ft_strcmp(rl, pars->cur_redir->delim))
		{
			free(rl);
			break ;
		}
		if (pars->cur_redir->expand == true)
			to_write = expand_str(rl, pars->mini);
		else
			to_write = ft_strdup(rl);
		free(rl);
		if (!to_write)
			return (1);
		write(fd, to_write, ft_strlen(to_write));
		write(fd, "\n", 1);
		free(to_write);
	}
	return (0);
}

char	*make_tmp_file(int i)
{
	char	*tmp_file;
	char	*index;

	index = ft_itoa(i);
	tmp_file = ft_strjoin("/tmp/mini_", index);
	free(index);
	if (!tmp_file)
		return (NULL);
	return (tmp_file);
}

int	heredoc_child(t_parser *pars, char *tmp_file)
{
	int		fd;

	set_default_signals(1);
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(tmp_file), 1);
	if (write_to_heredoc(pars, fd))
		return (close(fd), 1);
	close(fd);
	return (0);
}

int	handle_heredoc(t_parser *pars, int i)
{
	int					pid;
	struct sigaction	old_sa;

	pars->cur_redir->file = make_tmp_file(i);
	if (!pars->cur_redir->file)
		return (1);
	ignore_signals(&old_sa);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		if (heredoc_child(pars, pars->cur_redir->file))
			exit(1);
		exit(0);
	}
	else if (pid > 0)
	{
		if (wait_child(pars->mini, pid))
			return (free(pars->cur_redir->file),
				pars->cur_redir->file = NULL, 1);
		restore_old_signals(&old_sa);
	}
	return (0);
}

int	process_heredoc(t_parser *pars)
{
	int	i;

	i = 0;
	pars->cur_cmd = pars->mini->cmds;
	while (pars->cur_cmd)
	{
		pars->cur_redir = pars->cur_cmd->redirs;
		while (pars->cur_redir)
		{
			if (pars->cur_redir->type == T_HEREDOC && handle_heredoc(pars, i))
				return (1);
			pars->cur_redir = pars->cur_redir->next;
			i++;
		}
		pars->cur_cmd = pars->cur_cmd->next;
	}
	return (0);
}
