/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helper_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:18:58 by hgatarek          #+#    #+#             */
/*   Updated: 2025/08/06 18:27:53 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void restore_fds(int original_stdin, int original_stdout)
{
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}

bool is_parent_mod_builtins(char *cmd_name)
{
    if (!cmd_name)
        return (false);
    if (ft_strcmp(cmd_name, "cd") == 0
        || ft_strcmp(cmd_name, "export") == 0
        || ft_strcmp(cmd_name, "unset") == 0
        || ft_strcmp(cmd_name, "exit") == 0)
        return (true);
    return (false);
}
