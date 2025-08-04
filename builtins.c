/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 19:17:09 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/04 18:07:23 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *str)
{
	return (!ft_strcmp(str, "echo")
		|| !ft_strcmp(str, "cd")
		|| !ft_strcmp(str, "pwd")
		|| !ft_strcmp(str, "export")
		|| !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "env")
		|| !ft_strcmp(str, "exit"));
}
