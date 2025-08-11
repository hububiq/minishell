/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_extract.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:42:04 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/10 23:07:09 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_var_limits(t_expander	*exp)
{
	exp->var_start = exp->var_key - exp->str;
	exp->var_end = exp->var_start + 1;
	if (!exp->str[exp->var_end])
		return ;
	else if (exp->str[exp->var_end] == '$'
		|| exp->str[exp->var_end] == '?'
		|| exp->str[exp->var_end] == '0')
	{
		exp->var_end = exp->var_start + 2;
		return ;
	}
	while (exp->str[exp->var_end]
		&& !ft_isspace(exp->str[exp->var_end])
		&& !is_special(exp->str[exp->var_end])
		&& !is_quote(exp->str[exp->var_end]))
		exp->var_end++;
}

char	*find_var(t_expander *exp)
{
	t_env	*var;

	if (!exp->var_key)
		return (NULL);
	if (ft_strcmp(exp->var_key, "?") == 0)
		return (ft_itoa(exp->mini->last_exit_code));
	if (ft_strcmp(exp->var_key, "0") == 0)
		return (ft_strdup("minishell"));
	if (ft_strcmp(exp->var_key, "$") == 0)
		return (ft_itoa(exp->mini->mini_pid));
	var = find_env(exp->mini->env, exp->var_key);
	if (var && var->value)
		return (ft_strdup(var->value));
	else
		return (ft_strdup(""));
}

int	extract_fragments(t_expander *exp)
{
	if (!exp->str)
		return (1);
	exp->fragment_before = ft_substr(exp->str, 0, exp->var_start);
	exp->var_key = ft_substr(exp->str, exp->var_start + 1,
			exp->var_end - exp->var_start - 1);
	exp->fragment_after = ft_substr(exp->str, exp->var_end,
			ft_strlen(exp->str) - exp->var_end);
	if (!exp->fragment_before || !exp->var_key || !exp->fragment_after)
		return (1);
	exp->var_value = find_var(exp);
	if (!exp->var_value)
		return (1);
	return (0);
}

int	join_fragments(t_expander *exp)
{
	char	*temp;

	temp = ft_strjoin(exp->fragment_before, exp->var_value);
	if (!temp)
		return (1);
	exp->joint_str = ft_strjoin(temp, exp->fragment_after);
	free(temp);
	if (!exp->joint_str)
		return (1);
	return (0);
}
