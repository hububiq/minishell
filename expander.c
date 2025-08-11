/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 11:27:08 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/10 23:10:42 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_expander(t_expander *exp)
{
	if (!exp)
		return ;
	free(exp->fragment_before);
	free(exp->fragment_after);
	free(exp->var_value);
	free(exp->var_key);
	free(exp->joint_str);
}

bool	need_expansion(t_expander	*exp)
{
	if (exp->var_end == exp->var_start + 1)
		return (false);
	return (true);
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
