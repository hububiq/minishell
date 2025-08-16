/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:00:36 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/16 11:22:09 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_readline(void)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	unlink_tmp(t_data *mini)
{
	t_cmd	*cur_cmd;
	t_redir	*cur_redir;

	cur_cmd = mini->cmds;
	while (cur_cmd)
	{
		cur_redir = cur_cmd->redirs;
		while (cur_redir)
		{
			if (cur_redir->type == T_HEREDOC && cur_redir->file)
				unlink(cur_redir->file);
			cur_redir = cur_redir->next;
		}
		cur_cmd = cur_cmd->next;
	}
	return (0);
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
