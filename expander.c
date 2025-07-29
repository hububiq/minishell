/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:27:08 by mdziadko          #+#    #+#             */
/*   Updated: 2025/07/27 15:20:03 by mdziadko         ###   ########.fr       */
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
		&& !isspecial(exp->str[exp->var_end])
		&& !isquote(exp->str[exp->var_end]))
		exp->var_end++;
}

int	need_expansion(t_expander	*exp)
{
	if (exp->var_end == exp->var_start + 1)
		return (0);
	return (1);
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

int	free_expander(t_expander *exp)
{
	if (!exp)
		return (1);
	free(exp->fragment_before);
	free(exp->fragment_after);
	free(exp->var_value);
	free(exp->var_key);
	free(exp->joint_str);
	return (0);
}

char	*find_var(t_expander *exp)
{
	t_env	*var;

	if (!exp->var_key)
		return (NULL);
	if (ft_strcmp(exp->var_key, "?") == 0)
		return (ft_itoa(exp->mini->last_exit_code));
	if (ft_strcmp(exp->var_key, "0") == 0)
		return (ft_strdup("mini")); // argv[0]
	if (ft_strcmp(exp->var_key, "$") == 0)
		return (ft_itoa(exp->mini->mini_pid));
	var = find_env(exp->mini, exp->var_key);
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

char	*expand_str(char *src_str, t_data *mini)
{
	t_expander	exp;

	ft_bzero(&exp, sizeof(t_expander));
	exp.mini = mini;
	exp.str = ft_strdup(src_str);
	while ((size_t)exp.start < ft_strlen(exp.str))
	{
		exp.var_key = ft_strchr(&exp.str[exp.start], '$');
		if (!exp.var_key)
			break ;
		find_var_limits(&exp); 
		if (need_expansion(&exp))
		{
			if (extract_fragments(&exp) || join_fragments(&exp))
				return (free(exp.str), free_expander(&exp), NULL);
			free(exp.str);
			exp.str = ft_strdup(exp.joint_str);
			exp.start = exp.var_start + ft_strlen(exp.var_value);
			free_expander(&exp);
		}
		else
			exp.start = exp.var_end;
	}
	return (exp.str);
}

int	expand_tokens(t_data *mini)
{
	t_token	*token;
	char	*new_str;

	token = mini->tokens;
	while (token)
	{
		if (token->expand == true && token->type != T_HEREDOC)
		{
			new_str = expand_str(token->str, mini);
			if (!new_str)
				return (1);
			free(token->str);
			token->str = new_str;
		}
		token = token->next;
	}
	return (0);
}
