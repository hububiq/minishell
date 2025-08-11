/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 23:29:52 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/11 13:36:32 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
t_env	*new_env(char *str)
{
	t_env	*env;
	char	*equal;

	equal = ft_strchr(str, '=');
	if (!equal)
		return (NULL);
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->key = ft_strndup(str, equal - str);
	if (!env->key)
		return (free(env), NULL);
	env->value = ft_strdup(equal + 1);
	if (!env->value)
		return (free(env->key), free(env), NULL);
	env->next = NULL;
	return (env);
}
*/

char	*extract_env_key(char *str)
{
	char	*equal;

	if (!str)
		return (NULL);
	equal = ft_strchr(str, '=');
	if (!equal)
		return (ft_strdup(str));
	else
		return (ft_strndup(str, equal - str));
}

char	*extract_env_val(char *str)
{
	char	*equal;

	if (!str)
		return (NULL);
	equal = ft_strchr(str, '=');
	if (!equal)
		return (ft_strdup(""));
	else
		return (ft_strdup(equal + 1));
}

t_env	*new_env(char *str, bool exported)
{
	t_env	*env;

	if (!is_valid_env_key(str))
		return (NULL);
	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	ft_bzero(env, sizeof(t_env));
	env->key = extract_env_key(str);
	env->value = extract_env_val(str);
	if (!env->key || !env->value || !is_valid_env_key(env->key))
		return (free_env(env), NULL);
	env->exported = exported;
	env->next = NULL;
	return (env);
}

t_env	*find_env(t_env *head, char *key)
{
	t_env	*cur;

	cur = head;
	while (cur)
	{
		if (!key || ft_strcmp(key, cur->key) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*cur;
	int		i;

	if (!envp || !envp[0])
		return (NULL);
	head = new_env(envp[0], true);
	if (!head)
		return (NULL);
	cur = head;
	i = 1;
	while (envp[i])
	{
		tail = new_env(envp[i], true);
		if (!tail)
			return (free_env_list(head), NULL);
		cur->next = tail;
		cur = cur->next;
		i++;
	}
	return (head);
}
