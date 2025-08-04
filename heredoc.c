/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 21:02:29 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/04 18:04:28 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int	pid;

	pars->cur_redir->file = make_tmp_file(i);
	if (!pars->cur_redir->file)
		return (1);
	ignore_signals(NULL);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		set_default_signals();
		exit(heredoc_child(pars, pars->cur_redir->file));
	}
	if (wait_child(pars->mini, pid))
	{
		write(1, "\n", 1);
		free(pars->cur_redir->file);
		pars->cur_redir->file = NULL;
		return (1);
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

/*
static char	*read_line(char *prompt)
{
	char	*rl;
	char	*res;

	if (!prompt)
		return (NULL);
	write(STDIN_FILENO, prompt, ft_strlen(prompt));
	rl = get_next_line(STDIN_FILENO);
	if (!rl)
		return (NULL);
	res = ft_strndup(rl, ft_strlen(rl) - 1);
	free(rl);
	return (res);
}
*/