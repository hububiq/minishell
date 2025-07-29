/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:21:43 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/24 22:10:06 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*add_env(char *str)
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

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*cur;
	int		i;

	head = add_env(envp[0]);
	if (!head)
		return (NULL);
	cur = head;
	i = 1;
	while (envp[i])
	{
		tail = add_env(envp[i]);
		if (!tail)
			return (free_env_list(head), NULL);
		cur->next = tail;
		cur = cur->next;
		i++;
	}
	return (head);
}

t_env	*find_env(t_data *mini, char *key)
{
	t_env	*cur;

	cur = mini->env;
	while (cur)
	{
		if (!key || ft_strcmp(key, cur->key) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

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

t_data	*init_data(char **envp)
{
	t_data	*mini;

	mini = malloc(sizeof(t_data));
	if (!mini)
		return (NULL);
	ft_bzero(mini, sizeof(t_data));
	mini->env = init_env(envp);
	if (!mini->env)
		return (free(mini), NULL);
	mini->prompt = NULL;
	mini->cmds = NULL;
	mini->tokens = NULL;
	mini->exit_code = 0;
	mini->last_exit_code = 0;
	mini->last_signal = 0;
	mini->hd_signal = 0;
	//mini->mini_pid = ft_getpid();
	return (mini);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)	argc;
// 	(void) 	argv;
// 	t_data	*mini = init_data(envp);
// 	print_env(mini, NULL);
// 	printf("\n\n");
// 	print_env(mini, "USER");
// 	printf("\n\n");
// 	print_env(mini, "COMMAND_MODE");
// }