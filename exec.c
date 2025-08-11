/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:50:50 by hgatarek          #+#    #+#             */
/*   Updated: 2025/08/08 15:37:53 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*I only free whats been malloced before execve 
and after the fork. No freeing of data using by parent*/
void	execute_child_process(t_cmd *cmd, t_data *mini)
{
	char	**envp;

	if (!cmd->path)
	{
		write(STDERR_FILENO, "mini: ", 6);
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		exit(127);
	}
	envp = convert_env_list_to_array(mini->env);
	if (!envp)
		exit(1);
	execve(cmd->path, cmd->args, envp);
	report_cd_error(cmd->args[0]);
	free_arr(envp);
	exit(126);
}

void	apply_redirections(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
}

int	launch_command(t_cmd *cmd, t_data *mini, int prev_pipe_read_end,
					int pipe_fds[2])
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (report_error("fork failed", 1), 1);
	if (pid == 0)
	{
		if (prev_pipe_read_end != STDIN_FILENO)
		{
			dup2(prev_pipe_read_end, STDIN_FILENO);
			close(prev_pipe_read_end);
		}
		if (cmd->next)
		{
			close(pipe_fds[0]);
			dup2(pipe_fds[1], STDOUT_FILENO);
			close(pipe_fds[1]);
		}
		apply_redirections(cmd);
		if (cmd->builtin)
			exit(execute_builtin(cmd, mini));
		else
			execute_child_process(cmd, mini);
	}
	return (pid);
}

int	execute_pipeline(t_data *mini)
{
	t_cmd	*curr_cmd;
	int		prev_pip_rend;
	int		pipe_fds[2];

	curr_cmd = mini->cmds;
	prev_pip_rend = STDIN_FILENO;
	while (curr_cmd)
	{
		if (curr_cmd->next)
			if (pipe(pipe_fds) == -1)
				return (report_error("pipe failed", 1));
		curr_cmd->pid = launch_command(curr_cmd, mini, prev_pip_rend, pipe_fds);
		if (curr_cmd->pid == -1)
		{
			if (prev_pip_rend != STDIN_FILENO)
				close(prev_pip_rend);
			return (1);
		}
		manage_parent_pipes(&prev_pip_rend, pipe_fds, curr_cmd->next);
		curr_cmd = curr_cmd->next;
	}
	return (0);
}

int	execute(t_data *mini)
{
	t_cmd	*current_cmd;
	int		original_stdin;
	int		original_stdout;

	current_cmd = mini->cmds;
	if (!current_cmd)
		return (0);
	if (current_cmd && !current_cmd->next && current_cmd->builtin
		&& is_parent_mod_builtins(current_cmd->args[0]))
	{
		original_stdin = dup(STDIN_FILENO);
		original_stdout = dup(STDOUT_FILENO);
		apply_redirections(current_cmd);
		mini->last_exit_code = execute_builtin(current_cmd, mini);
		restore_fds(original_stdin, original_stdout);
		return (0);
	}
	execute_pipeline(mini);
	return (wait_for_children(mini));
}
