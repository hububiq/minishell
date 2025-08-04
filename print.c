/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:17:12 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/31 10:23:58 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_data *mini, char *key)
{
	t_env	*cur;

	cur = mini->env;
	while (cur)
	{
		if (!key || ft_strcmp(key, cur->key) == 0)
			printf("%s = %s\n", cur->key, cur->value);
		cur = cur->next;
	}
}

void	print_token(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp)
	{
		if (!temp)
		{
			printf("Empty token");
			return ;
		}
		printf("%s | %i | %i | %i\n",
			temp->str, temp->type, temp->expand, temp->word_id);
		temp = temp->next;
	}
}

void	print_cmd(t_data *mini)
{
	int		cmd_i;
	t_cmd	*cur_cmd;
	t_redir	*cur_redir;

	if (!mini->cmds)
		return ;
	cmd_i = 0;
	cur_cmd = mini->cmds;
	printf("\n-----PRINT COMMAND-----\n");
	while (cur_cmd)
	{
		printf("cmd %i : ", cmd_i);
		if (!cur_cmd->args || !cur_cmd->args[0])
			printf("(null)\n");
		else if (cur_cmd->args)
		{
			for (int i = 0; cur_cmd->args[i]; i++)
				printf("arg %d: %s\n", i + 1, cur_cmd->args[i]);
		}
		printf("path	: %s\n", cur_cmd->path);
		printf("is builtin: %i\n", cur_cmd->builtin);
		printf("fd_in	: %i\n", cur_cmd->fd_in);
		printf("fd_out	: %i\n", cur_cmd->fd_out);

		cur_redir = cur_cmd->redirs;
		if (!cur_redir)
			printf("redirs	: (none)\n");
		else
		{
			printf("redirs:\n");
			while (cur_redir)
			{
				printf("  type : %d | ", cur_redir->type);
				printf("delim : %s\n", cur_redir->delim);
				printf("file  : %s\n", cur_redir->file);
				if (cur_redir->type == T_HEREDOC)
				{
					int fd = open(cur_redir->file, O_RDONLY, 0644);
					char output[51];
					int bytes_read;
					while ((bytes_read = read(fd, output, 50)) > 0)
					{
						output[bytes_read] = '\0';
						printf("%s", output);
					}
					close(fd);
				}
				printf("expand : %d\n", cur_redir->expand);
				cur_redir = cur_redir->next;
			}
		}
		printf("----------------------------------\n");
		cmd_i++;
		cur_cmd = cur_cmd->next;
	}
}
