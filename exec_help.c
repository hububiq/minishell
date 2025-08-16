/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 21:58:18 by hgatarek          #+#    #+#             */
/*   Updated: 2025/08/16 11:29:37 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_for_children(t_data *mini)
{
	int		status;
	t_cmd	*cmd;

	cmd = mini->cmds;
	mini->last_exit_code = 0;
	if (!cmd)
		return (mini->last_exit_code);
	while (cmd)
	{
		if (cmd->pid > 0)
		{
			waitpid(cmd->pid, &status, 0);
			if (WIFEXITED(status))
				mini->last_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				mini->last_exit_code = 128 + WTERMSIG(status);
		}
		else if (cmd->redirs && (cmd->fd_in == -1 || cmd->fd_out == -1))
		{
			mini->last_exit_code = 1;
		}
		cmd = cmd->next;
	}
	return (mini->last_exit_code);
}

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

void	manage_parent_pipes(int *prev_pip_rend, int pipe_fds[2],
		bool has_next_cmd)
{
	if (*prev_pip_rend != STDIN_FILENO)
		close(*prev_pip_rend);
	if (has_next_cmd)
	{
		close(pipe_fds[1]);
		*prev_pip_rend = pipe_fds[0];
	}
}

char	**convert_env_list_to_array(t_env *env_list)
{
	char	**envp;
	t_env	*current_node;
	char	*temp_join;
	int		i;

	envp = (char **)malloc(sizeof(char *) * (count_env_nodes(env_list) + 1));
	if (!envp)
		return (report_error("malloc failed for envp array", 1), NULL);
	current_node = env_list;
	i = 0;
	while (current_node)
	{
		temp_join = ft_strjoin(current_node->key, "=");
		if (!temp_join)
			return (free_arr(envp), report_error("malloc fail", 1), NULL);
		envp[i] = ft_strjoin(temp_join, current_node->value);
		free(temp_join);
		if (!envp[i])
			return (free_arr(envp), report_error("malloc fail", 1), NULL);
		current_node = current_node->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int	count_env_nodes(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

/*last line - Reap any other zombie processes from the pipeline*/
/*first line - no external command or forked built-in was run*/
// void	wait_for_pipeline(t_data *mini)
// {
//     int status;

//     if (last_pid <= 0)
//         return;
//     while (mini->next)
//     waitpid(last_pid, &status, 0);
//     if (WIFEXITED(status))
//         mini->exit_code = WEXITSTATUS(status);
//     else if (WIFSIGNALED(status))
//         mini->exit_code = 128 + WTERMSIG(status);
//     while (wait(NULL) > 0);
// }
