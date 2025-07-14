/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 10:32:47 by mdziadko          #+#    #+#             */
/*   Updated: 2025/05/18 11:05:39 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include <stdbool.h> 

# define DQUOTE 34
# define SQUOTE 39

typedef enum e_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
	T_QUOTE
}	t_type;

typedef struct s_token
{
	char			*str;
	t_type			type;
	bool			expand;
	int				word_id;
	struct s_token	*next;
}					t_token;

typedef struct s_redir
{
	t_type			type;
	union
	{
		char	*file;
		char	*delim;
	};
	struct s_redir	*next;	
}					t_redir;

typedef struct s_cmd
{
	char			*path;
	char			**args;
	bool			builtin;
	int				fd_in;
	int				fd_out;
	t_redir			*redirs;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	t_env	*env;
	char	*prompt;
	t_token	*tokens;
	t_cmd	*cmds;
	int		exit_code;
	int		last_signal;
	int		hd_signal;
}			t_data;

typedef struct s_lexer
{
	t_data	*mini;
	int		i;
	bool	expand;
	int		space;
	t_token	*tail;
}			t_lexer;

typedef struct s_expander
{
	t_data	*mini;
	t_token	*token;
	char	*str;
	int		start;
	int		var_start;
	int		var_end;
	char	*var_key;
	char	*var_value;
	char	*fragment_before;
	char	*fragment_after;
	char	*joint_str;
}			t_expander;

typedef struct s_parser
{
	t_data	*mini;
	t_cmd	*cur_cmd;
	t_token	*cur_token;
	t_redir	*cur_redir;
	int		arg_count;
	int		arg_i;
}			t_parser;

#endif
