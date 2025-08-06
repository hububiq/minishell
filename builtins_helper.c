/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:37:51 by hgatarek          #+#    #+#             */
/*   Updated: 2025/08/05 20:00:00 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_env_value(t_env *env_list, char *key)
{
    t_env *current_node;

    if (!key || !env_list)
        return (NULL);
    current_node = env_list;
    while (current_node)
    {
        if (current_node->key && ft_strcmp(key, current_node->key) == 0)
            return (current_node->value);
        current_node = current_node->next;
    }
    return (NULL);
}

int add_new_env_var(t_env **env_list_head, char *key, char *value)
{
    t_env   *current;
    char    *full_str;
    char    *temp_str;

    temp_str = ft_strjoin(key, "=");
    full_str = ft_strjoin(temp_str, value);
    free(temp_str);
    if (!full_str)
        return (report_error("malloc failed", 1));
    if (*env_list_head == NULL)
        *env_list_head = add_env(full_str);
    else
    {
        current = *env_list_head;
        while (current->next)
            current = current->next;
        current->next = add_env(full_str);
    }
    free(full_str);
    return (0);
}

int update_existing_env(t_env *env_list, char *key, char *value)
{
    char    *new_value;

    while (env_list)
    {
        if (env_list->key && ft_strcmp(key, env_list->key) == 0)
        {
            new_value = ft_strdup(value);
            if (!new_value)
            {
                report_error("malloc failed", 1);
                return (-1);
            }
            free(env_list->value);
            env_list->value = new_value;
            return (1);
        }
        env_list = env_list->next;
    }
    return (0);
}

int set_env_var(t_env **env_list_head, char *key, char *value)
{
    int update_status;

    if (!key || !value || !env_list_head)
        return (1);
    update_status = update_existing_env(*env_list_head, key, value);
    if (update_status == 1)
        return (0);
    if (update_status == -1)
        return (1);
    return (add_new_env_var(env_list_head, key, value));
}
