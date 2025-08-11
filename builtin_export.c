/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 19:17:09 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/11 13:31:38 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	EXPORT KEY=VALUE
	Add/update environment variable.
-------------------------------------------------------------------------------
	EXPORT (No value)
	Just print sorted env (export without args).

	EXPORT KEY (no =)
	If KEY exists → just mark exported = true.
	If KEY does not exist → add it with empty value ("") and exported = true.

	EXPORT KEY=VALUE
	Add/update KEY and mark exported = true.
	This is permanent in your environment.

	KEY=VALUE (at the prompt (no export))
	This is a shell variable, not yet exported → exported = false.
	It won’t appear in env or be passed to child proc until export is called.
*/

bool	arg_is_env(char *str)
{
	if (ft_strchr(str, '=') && is_valid_env_key(str))
		return (true);
	return (false);
}

void	append_env(t_env **head, t_env *new)
{
	t_env	*cur;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

int	export_env(t_env *head, char *str)
{
	t_env	*temp;
	char	*key;

	if (!is_valid_env_key(str))
		return (report_error("export: not a valid identifier", 1));
	key = extract_env_key(str);
	if (!key)
		return (1);
	temp = find_env(head, key);
	free(key);
	if (!temp)
	{
		temp = new_env(str, true);
		append_env(&head, temp);
		return (0);
	}
	else if (ft_strchr(str, '='))
	{
		free(temp->value);
		temp->value = extract_env_val(str);
		if (!temp->value)
			return (1);
	}
	temp->exported = true;
	return (0);
}

int	builtin_export(t_cmd *cmd, t_data *mini)
{
	int	i;

	if (!mini || !cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (!cmd->args[1])
	{
		print_exported_env(mini->env, NULL);
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (export_env(mini->env, cmd->args[i]))
			return (1);
		i++;
	}
	return (0);
}
