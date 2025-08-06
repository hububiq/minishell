/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:17:44 by hgatarek          #+#    #+#             */
/*   Updated: 2025/08/06 18:25:10 by hgatarek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*message piece by piece with write calls*/
void report_cd_error(char *path)
{
    write(STDERR_FILENO, "mini: ", 6);
    write(STDERR_FILENO, ": cd: ", 6);
    if (path)
        write(STDERR_FILENO, path, ft_strlen(path));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
    write(STDERR_FILENO, "\n", 1);
}

char *get_cd_path(t_cmd *cmd, t_data *mini)
{
    char *path;

    if (!cmd->args[1])
    {
        path = find_env_value(mini->env, "HOME");
        if (!path)
            report_error("cd: HOME not set", 1);
        else if (*path == '\0')
            return (".");
        return (path);
    }
    return (cmd->args[1]);
}

int update_pwd_vars(t_data *mini, char *old_pwd_path)
{
    char    new_pwd[4096];
    
    if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
    {
        report_error("cd: error retrieving current directory path", 1);
        set_env_var(&mini->env, "OLDPWD", old_pwd_path);
        return (1);
    }
    set_env_var(&mini->env, "OLDPWD", old_pwd_path);
    set_env_var(&mini->env, "PWD", new_pwd);
    return (0);
}
