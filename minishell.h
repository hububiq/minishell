/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgatarek <hgatarek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 10:32:47 by mdziadko          #+#    #+#             */
/*   Updated: 2025/08/06 18:32:48 by hgatarek         ###   ########.fr       */
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
# include <signal.h>

# define DQUOTE 34
# define SQUOTE 39
# define GREEN "\033[1;32m"
# define CYAN "\033[1;36m"
# define RESET "\033[0m"

extern volatile sig_atomic_t	g_sig;

typedef enum e_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
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
	bool			expand;
	char			*file;
	char			*delim;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			*path;
	char			**args;
	bool			builtin;
	int				fd_in;
	int				fd_out;
	int				pid;
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
	int		last_exit_code;
	int		mini_pid;
}			t_data;

typedef struct s_lexer
{
	t_data	*mini;
	char	*prompt;
	int		i;
	bool	expand;
	int		space;
	t_token	*tail;
}			t_lexer;

typedef struct s_expander
{
	t_data	*mini;
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

// MAIN
int		run_prompt(t_data *mini);
void	run_mini(t_data	*mini);

// INIT
t_env	*add_env(char *str);
t_env	*init_env(char **envp);
t_env	*find_env(t_data *mini, char *key);
t_data	*init_data(char **envp);

// SIGNAL HANDLER
void	setup_signals(void);
void	ignore_signals(struct sigaction *old_sa);
void	sigint_handler(int signo);
void	set_default_signals(void);

// LEXER
t_token	*add_token(char *str, t_type type, bool expand);
int		increment_word_id(t_lexer *lex, t_token *token);
t_token	*get_next_token(t_lexer *lex);
int		init_lexer(t_data *mini, t_lexer *lx);
int		tokenize_input(t_data *mini);

// LEXER_EXTARCT
char	*extract_word(t_lexer *lex);
char	*extract_quotes(t_lexer *lex);
char	*extract_delim(t_lexer *lex);

// LEXER_HELP
int		ft_isspecial(char c);
int		isquote(char c);
int		skip_whitespace(t_lexer *lex);

// VALIDATOR
int		syntax_error(char *msg);
int		cmd_error(char *msg);
int		report_error(char *msg, int code);
int		validate_syntax(t_data *mini);

// EXPANDER
int		need_expansion(t_expander	*exp);
void	free_expander(t_expander *exp);
char	*expand_str(char *src_str, t_data *mini);
int		expand_tokens(t_data *mini);

// EXPANDER_EXTRACT
void	find_var_limits(t_expander	*exp);
char	*find_var(t_expander *exp);
int		extract_fragments(t_expander *exp);
int		join_fragments(t_expander *exp);

// PARSER
int		init_pars(t_data *mini, t_parser *pars);
int		parse_cmds(t_data *mini);

// PARSER_HELP
int		find_executable(t_cmd *cmd, char **dirs);
char	*find_cmd_path(t_parser *pars);
int		count_cmd_args(t_token *token);
char	*join_tokens_by_id(t_token **token);

// PARSER_PROCESS
int		handle_pipe(t_parser *pars);
int		handle_cmd(t_parser *pars);
t_cmd	*add_cmd(t_parser *pars);
int		add_arg(t_parser *pars);
int		add_redir(t_parser *pars, t_type type);

// HEREDOC
int		write_to_heredoc(t_parser *pars, int fd);
int		heredoc_child(t_parser *pars, char *tmp_file);
char	*make_tmp_file(int i);
int		handle_heredoc(t_parser *pars, int i);
int		process_heredoc(t_parser *pars);

// REDIRECTIONS
void	safe_close(int *fd);
int		open_file(t_redir *redir);
int		handle_cmd_redirs(t_parser *pars);
int		process_redirs(t_parser *pars);

// EXEC
bool 	is_parent_mod_builtins(char *cmd_name);
int 	execute(t_data *mini);
void	execute_child_process(t_cmd *cmd, t_data *mini);
void 	apply_redirections(t_cmd *cmd);
int		launch_command(t_cmd *cmd, t_data *mini, int prev_pipe_read_end,
                        int pipe_fds[2]);
int		execute_pipeline(t_data *mini);

// EXEC_HELP
int		wait_child(t_data *mini, int child_pid);
int		wait_for_children(t_data *mini);
char 	*get_cd_path(t_cmd *cmd, t_data *mini);
int 	update_pwd_vars(t_data *mini, char *old_pwd_path);
void	manage_parent_pipes(int *prev_pip_rend, int pipe_fds[2],
								bool has_next_cmd);
char 	**convert_env_list_to_array(t_env *env_list);

// BUILTINS
bool	is_builtin(char *str);
void 	restore_fds(int original_stdin, int original_stdout);
void	report_cd_error(char *path);
int		builtin_env(t_cmd *cmd, t_data *mini);
int		builtin_cd(t_cmd *cmd, t_data *mini);
int		builtin_pwd(t_cmd *cmd);
int 	builtin_echo(t_cmd *cmd);
int 	execute_builtin(t_cmd *cmd, t_data *mini);
int		count_env_nodes(t_env *env_list);
int		set_env_var(t_env **env_list_head, char *key, char *value);
int		update_existing_env(t_env *env_list, char *key, char *value);
int		set_env_var(t_env **env_list_head, char *key, char *value);
int		add_new_env_var(t_env **env_list_head, char *key, char *value);
char	*find_env_value(t_env *env_list, char *key);

// CLEANUP_0
void	free_program(t_data *mini);
void	free_data(t_data *mini);
void	free_token_list(t_token	*token);
void	free_redir_list(t_redir *redir);
void	free_cmd_list(t_cmd *cmd);

// CLEANUP_1
void	reset_readline(void);
int		unlink_tmp(t_data *mini);
void	free_arr(char **arr);
void	free_env(t_env *env);
void	free_env_list(t_env *env);

// PRINT 
void	print_env(t_data *mini, char *key);	//MUST STAY
void	print_cmd(t_data *mini);	//REMOVE IT LATER
void	print_token(t_token *token);	//REMOVE IT LATER

#endif
