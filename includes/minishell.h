/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: rodantec <rodantec@student.s19.be>         +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/05/28 11:26:56 by rodantec          #+#    #+#             */
/*   Updated: 2025/05/28 11:26:56 by rodantec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

/* Structures de base définies séparément pour compatibilité C */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	int		count_env;
	int		count_export;
	char	**env;
	char	*oldpwd;
	char	**export;
}	t_env;

typedef struct s_command
{
	char		**args;				/* Arguments de la commande (argv) */
	t_token		*redirections;		/* Liste des tokens de redirection */
}	t_command;

typedef struct s_pipeline
{
	t_command	**commands;
	int			cmd_count;
	int			**pipes;
}	t_pipeline;

typedef struct s_parse_context
{
	int		arg_count;				
	int		redirection_count;		
	char	**temp_args;			
	t_token	*temp_redirections;		
}	t_parse_context;

typedef struct s_shell_state
{
	int		shell_status;
	int		exit_status;
	pid_t	child_pid;
}	t_shell_state;

typedef struct s_minishell
{
	t_token			*tokens;			
	t_env			env;				
	t_command		**commands;			
	t_pipeline		*pipeline;		
	t_parse_context	parse_ctx;			
	t_shell_state	state;
}	t_minishell;

extern int	g_signal_received;

typedef struct s_quote_state
{
	int		in_quotes;
	char	quote_type;
}	t_quote_state;

typedef struct s_dollar_context
{
	char	*current;
	int		i;
	char	quote_type;
	t_env	*envp;
}	t_dollar_context;

typedef struct s_process_context
{
	char			*input;
	int				*i;
	t_token			**tokens;
	t_env			*envp;
	t_shell_state	*state;
}	t_process_context;

typedef struct s_exec_context
{
	t_token			*token;
	t_env			*env;
	t_command		*cmd;
	t_pipeline		*pipeline;
	t_shell_state	*state;
}	t_exec_context;

typedef struct s_init_context
{
	t_minishell		*minishell;
	t_pipeline		*pipeline;
	t_env			*envp;
	t_token			*token_lexer;
	t_shell_state	*state;
}	t_init_context;

typedef struct s_command_logic_context
{
	t_token			*tokens;
	t_command		*cmd;
	t_env			*envp;
	t_shell_state	*state;
	t_exec_context	*ctx;
}	t_command_logic_context;

typedef struct s_variable_replacement_context
{
	char			*current;
	char			*temp_word;
	int				i;
	t_env			*envp;
	t_shell_state	*state;
}	t_variable_replacement_context;

typedef struct s_context_init_params
{
	char			*current;
	char			*temp_word;
	int				i;
	t_env			*envp;
	t_shell_state	*state;
}	t_context_init_params;

typedef struct s_scan_expand_params
{
	char			*current;
	char			*temp_word;
	int				start_i;
	t_env			*envp;
	t_shell_state	*state;
}	t_scan_expand_params;

typedef struct s_redirection_params
{
	t_token			*token;
	t_env			*envp;
	t_token			*token_lexer;
	t_pipeline		*pipeline;
	t_shell_state	*state;
}	t_redirection_params;

typedef struct s_heredoc_params
{
	char			*delimiter;
	int				should_expand;
	t_env			*envp;
	t_shell_state	*state;
}	t_heredoc_params;

typedef struct s_heredoc_content_params
{
	int				fd;
	char			*delimiter;
	int				should_expand;
	t_env			*envp;
	t_shell_state	*state;
}	t_heredoc_content_params;

/* ========================================================================== */
/*                            FONCTIONS LEXER                               */
/* ========================================================================== */
t_token			*lexer(char *input, t_env *envp, t_shell_state *state);
t_token			*create_token(t_token_type type, char *value);
void			add_token(t_token **head, t_token *new_token);
void			free_tokens(t_token **tokens);
void			print_token(t_token *token);
void			print_tokens(t_token *tokens);
int				count_tokens(t_token *tokens);
void			handle_quotes(char c, int *in_quotes, char *quote_type);
int				check_word_end(char *input, int i, int in_quotes);
int				process_special_redirections(char *input, int *i,
					t_token **tokens);
int				is_separator(char c);
t_token_type	get_token_type(char c);
int				process_word(t_process_context *ctx);
int				process_redirection(char *input, int *i, t_token **tokens);
int				process_redirections(t_redirection_params *params,
					int stdin_backup, int stdout_backup);
int				process_pipe(char *input, int *i, t_token **tokens);
int				process_lexer_token(char *input, int *i, t_token **tokens,
					t_process_context *ctx);

int				init_env(t_env *envp, char **env);
void			free_env(t_env *envp);

char			**tokens_to_args(t_token *token);
void			execute_cmd(t_token *token, t_env *env, t_shell_state *state);

int				redirect_input(char *filename);

/* free_all.c */
void			free_pipeline(t_pipeline *pipeline);
void			free_token_list(t_token *tokens);
void			cleanup_minishell(void);
int				contains_redirection(t_token *tokens);
int				handle_redirections(t_redirection_params *params);
int				apply_single_redirection(t_redirection_params *params);
int				redirect_output_trunc(char *filename);
int				redirect_output_append(char *filename);
char			*handle_heredoc(t_heredoc_params *params);
void			cleanup_heredoc(char *filename);

char			*expand(char *arg, t_env *envp, t_shell_state *state);
char			*expand_once(char *arg, t_env *envp, t_shell_state *state);
char			*expand_variables(char *temp_word, t_env *envp,
					t_shell_state *state);
char			*process_quotes_and_expand(char *word, t_env *envp,
					t_shell_state *state);
char			*extract_var_name(char *str);
char			*get_env_value(char *var_name, t_env *envp,
					t_shell_state *state);
char			*handle_expansion(char *prefix, char *var_value, char *suffix);
char			*remove_quotes(char *str);
char			*finalize_expansion(char *new_str, char *temp_word, int j);
int				find_var_end(char *str, int start);
void			update_quote_state(char c, char *quote_type, int *in_quotes);
int				is_valid_variable(char *str, int i);
char			*process_expansion_loop(char *current,
					t_env *envp, int *found_var);
char			*build_replacement_string(char *prefix, char *var_value);
char			*replace_variable(char *temp_word, int i,
					t_env *envp, t_shell_state *state);
char			*handle_variable_replacement(
					t_variable_replacement_context *ctx);
char			*process_variable_expansion(
					t_variable_replacement_context *ctx);
void			init_scan_context(t_variable_replacement_context *ctx,
					t_context_init_params *params);
char			*process_variable_in_loop(
					t_context_init_params *params);
void			setup_params(t_context_init_params *params,
					t_scan_expand_params *scan_params, int i);
char			*handle_variable_processing(
					t_variable_replacement_context *ctx);
char			*scan_and_expand(t_scan_expand_params *params);

void			setup_interactive_signals(void);
void			setup_child_signals(void);
void			setup_execution_signals(void);

// Error handling functions
void			print_command_not_found(char *cmd);
void			print_syntax_error(void);
void			print_syntax_error_token(char *token, t_shell_state *state);
int				error_token(t_token *token, t_shell_state *state);
int				check_syntax_errors(t_token *tokens, t_shell_state *state);
int				run_builtin(char **args, t_env *envp, t_shell_state *state);
t_command		*parse_line(char *line, t_env *envp, t_shell_state *state);
void			free_tokens(t_token **tokens);
void			restore_file_descriptors(int stdin_backup, int stdout_backup);
int				save_file_descriptors(int *stdin_backup, int *stdout_backup);
void			free_cmd(t_command *cmd);
int				init_command(t_token *token, t_command **cmd);
int				is_redirection(t_token *token);

/* ========================================================================== */
/*                            FONCTIONS BUILT-IN                            */
/* ========================================================================== */

int				count_env(char **env, t_env *envp);
int				echo(char **args);
int				execute_builtin(t_token *token, t_env *envp,
					t_shell_state *state);
int				exit_shell(char **args, t_shell_state *state);
int				export_var(t_env *envp, char **args);
int				cd(char **args, t_env *envp);
int				pwd(void);
int				print_env(t_env *envp);
int				unset_var(char **args, t_env *envp);
void			sort_env(char **env, int count);
char			**env_cpy(t_env *envp);
int				update_env(t_env *envp, char *arg, char *value);
int				find_env_var_index(char **env, char *arg);
void			print_sorted_env(int count, t_env *envp);
char			*ft_strjoin3(char *s1, char *s2, char *s3);
int				is_valid_identifier(char *str);
int				is_valid_value(char *str);
int				update_env_var(t_env *envp, char *var);
char			*get_var_name(char *str);
char			*get_var_value(char *str);
int				has_plus_equals(char *str);
char			*get_current_value(char **env, char *var_name);
int				has_equals_sign(char *str);
char			*join_if_append(char *value, char *current);
int				update_export_var(t_env *envp, char *var);
int				is_builtin(char *cmd);

/* Utils env functions */
char			*update_shlvl(char **env);
char			**create_default_env(void);
int				init_count(t_env *envp, char **env);
int				init_env_vars(t_env *envp, char **env, int i);
int				init_export_vars(t_env *envp, int i);

/* ========================================================================== */
/*                            FONCTIONS EXEC                                */
/* ========================================================================== */

char			**get_path_cut(t_env *env);
char			*find_path(char *cmd, t_env *env);
void			first_child(t_exec_context *ctx);

void			free_array(char **array);

/* ========================================================================== */
/*                            FONCTIONS PIPELINE                            */
/* ========================================================================== */

/* pipeline_utils1.c */
void			close_pipes(int **pipes, int cmd_count);
int				**init_pipe_array(int cmd_count);
int				**create_pipes(int cmd_count);
void			setup_pipes(int **pipes, int cmd_index, int cmd_count);

/* pipeline_utils2.c */
t_token			*create_command_token(t_command *cmd);
void			execute_external_command(t_command *cmd,
					t_minishell *minishell, t_token *token);
void			execute_pipeline_command(t_command *cmd,
					int cmd_index, t_minishell *minishell);
void			handle_builtin_execution(t_command *cmd,
					t_minishell *minishell, t_token *token);

/* pipeline_utils3.c */
int				count_commands(t_token *tokens);
int				count_args(t_token *cmd_start);
int				copy_command_args_only(t_command *cmd, t_token *cmd_start);
int				init_command_args(t_command *cmd, t_token *cmd_start,
					int arg_count);
int				process_child_wait(pid_t pid, int *last_status);
void			init_minishell_struct(t_init_context *ctx);
int				execute_pipeline_children(t_pipeline *pipeline, pid_t *pids,
					t_minishell *minishell);

/* create_pipeline.c */
t_command		*init_single_command(t_token *cmd_start);
t_pipeline		*init_pipeline_struct(int cmd_count);
t_pipeline		*create_pipeline(t_token *tokens, t_shell_state *state);

/* pipeline.c */
int				init_pipeline_execution(t_pipeline *pipeline, pid_t **pids);
int				handle_fork_error(t_pipeline *pipeline, pid_t *pids, int i);
int				wait_for_children(t_pipeline *pipeline,
					pid_t *pids, t_shell_state *state);
int				execute_pipeline(t_pipeline *pipeline,
					t_env *envp, t_token *token_lexer, t_shell_state *state);
void			cleanup_minishell(void);
int				check_redirection_error(t_token *current, t_shell_state *state);
int				check_pipe_error(t_token *current, t_shell_state *state);
void			setup_execution_signals(void);
void			sigquit_handler(int sig);
void			sigint_handler(int sig);
void			sigtstp_handler(int sig);
int				is_quoted_delimiter(char *delim);
char			*remove_quotes_from_delimiter(char *delim);
char			*create_heredoc_filename(void);
void			write_heredoc_content(t_heredoc_content_params *params);
void			restore_fds_and_exit(int *saved_fds, t_minishell *minishell);
void			setup_command_io(t_pipeline *pipeline,
					int cmd_index, int *saved_fds);
void			init_shell_state(t_shell_state *state);
int				process_line(char *line, t_env *envp, t_shell_state *state);
void			execute_single_command(t_token *tokens, t_env *envp,
					t_pipeline *pipeline, t_shell_state *state);

int				find_command_token(t_token *tokens);
char			*prompt_and_read(t_shell_state *state);
void			execute_command_logic(t_command_logic_context *ctx);
char			*process_dollar_sign(char *current, int i,
					t_env *envp, t_shell_state *state);
char			*create_prefix_and_cleanup(char *temp_word,
					int i, char *var_name);
char			*expand_loop(char *result, t_env *envp, t_shell_state *state);
char			*check_absolute_path(char *cmd);
int				handle_heredoc_redirect(t_token *token, t_env *envp,
					t_shell_state *state);
int				handle_child_status(int status);
void			handle_builtin_cmd(t_token *tokens, t_command *cmd,
					t_env *envp, t_shell_state *state);
#endif