/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_help.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 22:59:20 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/17 16:42:34 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	delete_env(t_env **head, char *key)
{
	t_env	*prev;
	t_env	*cur;

	if (!head || !*head || !key)
		return (1);
	prev = NULL;
	cur = *head;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (!prev)
				*head = cur->next;
			else
				prev->next = cur->next;
			free_env(cur);
			break ;
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

bool	is_valid_env_key(char *arg)
{
	int	i;

	i = 0;
	if (!arg || arg[i] == '=' || ft_isdigit(arg[i]))
		return (false);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

void	print_env(t_env *head, char *key)
{
	t_env	*cur;

	cur = head;
	while (cur)
	{
		if (!key || ft_strcmp(key, cur->key) == 0)
			printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
}

void	print_exported_env(t_env *head, char *key)
{
	t_env	*cur;

	cur = head;
	while (cur)
	{
		if (cur->exported == true)
		{
			if (!key || ft_strcmp(key, cur->key) == 0)
				printf("declare -x %s=\"%s\"\n", cur->key, cur->value);
		}
		cur = cur->next;
	}
}
