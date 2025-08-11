/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   builtin_cd.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/08/10 23:44:57 by mdziadko		  #+#	#+#			 */
/*   Updated: 2025/08/10 23:45:40 by mdziadko		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

/*message piece by piece with write calls*/
void	report_cd_error(char *path)
{
	write(STDERR_FILENO, "mini: ", 6);
	write(STDERR_FILENO, ": cd: ", 6);
	if (path)
		write(STDERR_FILENO, path, ft_strlen(path));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	write(STDERR_FILENO, "\n", 1);
}

char	*get_cd_path(t_cmd *cmd, t_data *mini)
{
	char	*path;

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

int	update_pwd_vars(t_data *mini, char *old_pwd_path)
{
	char	new_pwd[4096];

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

int	builtin_cd(t_cmd *cmd, t_data *mini)
{
	char	*path;
	char	old_pwd[4096];

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
