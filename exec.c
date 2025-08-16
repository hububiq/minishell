/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:50:50 by hgatarek          #+#    #+#             */
/*   Updated: 2025/08/16 11:29:18 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*I only free whats been malloced before execve 
and after the fork. No freeing of data using by parent*/
void	execute_child_process(t_cmd *cmd, t_data *mini)
{
	char	**envp;

	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (!cmd->path)
	{
		write(STDERR_FILENO, "mini: ", 6);
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

int	apply_redirections(t_cmd *cmd)
{
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		return (1);
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			return (perror("dup2"), 1);
		close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2"), 1);
		close(cmd->fd_out);
	}
	return (0);
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
		if (apply_redirections(cmd) != 0)
			exit (1);
		if (cmd->builtin)
			exit(execute_builtin(cmd, mini));
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
	pipe_fds[0] = -1;
	pipe_fds[1] = -1;
	while (curr_cmd)
	{
		mini->last_exit_code = 0;
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
		manage_parent_pipes(&prev_pip_rend, pipe_fds, (curr_cmd->next != NULL));
		curr_cmd = curr_cmd->next;
	}
	return (wait_for_children(mini));
}

int	execute(t_data *mini)
{
	t_cmd	*current_cmd;
	int		original_stdin;
	int		original_stdout;

	current_cmd = mini->cmds;
	if (!current_cmd)
		return (0);
	mini->last_exit_code = 0;
	if (current_cmd && !current_cmd->next && current_cmd->builtin
		&& is_parent_mod_builtins(current_cmd->args[0]))
	{
		original_stdin = dup(STDIN_FILENO);
		original_stdout = dup(STDOUT_FILENO);
		if (apply_redirections(current_cmd) != 0)
		{
			restore_fds(original_stdin, original_stdout);
			return (mini->last_exit_code = 1);
		}
		else
			mini->last_exit_code = execute_builtin(current_cmd, mini);
		restore_fds(original_stdin, original_stdout);
		return (mini->last_exit_code);
	}
	return (execute_pipeline(mini));
}
