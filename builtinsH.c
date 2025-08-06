/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtinsH.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:35:31 by hgatarek          #+#    #+#             */
/*   Updated: 2025/08/06 18:36:55 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_env(t_cmd *cmd, t_data *mini)
{
    t_env *current_env;

    if (cmd->args[1] != NULL)
    {
        report_error("env: my minishell takes no arguments", 1);
        return (1);
    }
    current_env = mini->env;
    while (current_env)
    {
        if (current_env->value != NULL)
        {
            printf("%s=%s\n", current_env->key, current_env->value);
        }
        current_env = current_env->next;
    }
    return (0);
}

int builtin_cd(t_cmd *cmd, t_data *mini)
{
    char    *path;
    char    old_pwd[4096];

    if (cmd->args[1] && cmd->args[2])
        return (report_error("cd: too many arguments", 1), 1);
    if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
        ft_strlcpy(old_pwd, "", sizeof(old_pwd));
    path = get_cd_path(cmd, mini);
    if (!path)
        return (1);
    if (chdir(path) != 0)
    {
        report_cd_error(path);
        return (1);
    }
    return (update_pwd_vars(mini, old_pwd));
}

int builtin_pwd(t_cmd *cmd)
{
    char cwd[4096];

    (void)cmd;
    getcwd(cwd, sizeof(cwd));
    // if (cwd)
    // {
        printf("%s\n", cwd);
        return (0);
    //}
    // else
    // {
    //     perror("minishell: pwd");
    //     return (1);
    // }
}

int builtin_echo(t_cmd *cmd)
{
    int     i;
    bool    print_newline;

    i = 1;
    print_newline = true;
    while (cmd->args[i] && ft_strncmp(cmd->args[i], "-n", 2) == 0)
    {
        print_newline = false;
        i++;
    }
    while (cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            printf(" ");
        i++;
    }
    if (print_newline)
        printf("\n");
    return (0);
}

int execute_builtin(t_cmd *cmd, t_data *mini)
{
    int exit_status;

    if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
        exit_status = builtin_echo(cmd);
    else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
        exit_status = builtin_pwd(cmd);
    else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
        exit_status = builtin_cd(cmd, mini);
    // else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
    //     exit_status = builtin_export(cmd, mini);
    // else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
    //     exit_status = builtin_unset(cmd, mini);
    else if (ft_strcmp(cmd->args[0], "env") == 0)
        exit_status = builtin_env(cmd, mini);
    // else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
    //     exit_status = builtin_exit(cmd, mini);
    mini->last_exit_code = exit_status;
    return (exit_status);
}
