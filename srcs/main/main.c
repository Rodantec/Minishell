/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodantec <rodantec@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:10:41 by rodantec          #+#    #+#             */
/*   Updated: 2025/06/23 12:50:00 by rodantec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Variable globale supprimée - utilisation de t_shell_state locale

void	handle_redir_only(t_token *tokens, t_env *envp, t_shell_state *state)
{
	int						result;
	int						saved_stdin;
	int						saved_stdout;
	t_redirection_params	redir_params;

	result = 0;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	redir_params.token = tokens;
	redir_params.envp = envp;
	redir_params.token_lexer = tokens;
	redir_params.pipeline = NULL;
	redir_params.state = state;
	if (handle_redirections(&redir_params) == 0)
		result = 0;
	else
		result = 1;
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
	state->exit_status = result;
}

int	check_and_handle_path(t_command *cmd, t_env *envp, t_shell_state *state)
{
	char	*path;

	if (!is_builtin(cmd->args[0]))
	{
		path = find_path(cmd->args[0], envp);
		if (!path)
		{
			print_command_not_found(cmd->args[0]);
			state->exit_status = 127;
			free_array(cmd->args);
			free(cmd);
			return (0);
		}
		free(path);
	}
	return (1);
}

void	handle_builtin_cmd(t_token *tokens, t_command *cmd,
			t_env *envp, t_shell_state *state)
{
	int						result;
	int						saved_stdout;
	int						saved_stdin;
	t_redirection_params	redir_params;

	result = run_builtin(cmd->args, envp, state);
	if (contains_redirection(tokens) > 0)
	{
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
		redir_params = (t_redirection_params){tokens,
			envp, tokens, NULL, state};
		if (handle_redirections(&redir_params) != 0)
			result = 1;
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
	}
	state->exit_status = result;
}

void	execute_single_command(t_token *tokens,
			t_env *envp, t_pipeline *pipeline, t_shell_state *state)
{
	t_command				*cmd;
	t_exec_context			ctx;
	t_command_logic_context	logic_ctx;

	if (init_command(tokens, &cmd) < 0)
		return ;
	if (!check_and_handle_path(cmd, envp, state))
		return ;
	ctx.token = tokens;
	ctx.env = envp;
	ctx.cmd = cmd;
	ctx.pipeline = pipeline;
	ctx.state = state;
	logic_ctx.tokens = tokens;
	logic_ctx.cmd = cmd;
	logic_ctx.envp = envp;
	logic_ctx.state = state;
	logic_ctx.ctx = &ctx;
	execute_command_logic(&logic_ctx);
	free_array(cmd->args);
	free(cmd);
}

int	main(int argc, char **argv, char **env)
{
	t_env			envp;
	t_shell_state	state;
	char			*line;

	(void)argc;
	(void)argv;
	memset(&envp, 0, sizeof(t_env));
	init_env(&envp, env);
	init_shell_state(&state);
	setup_interactive_signals();
	while (1)
	{
		line = prompt_and_read(&state);
		if (!line)
			break ;
		process_line(line, &envp, &state);
		free(line);
		if (state.shell_status == 1)
		{
			rl_clear_history();
			free_env(&envp);
			exit(state.exit_status);
		}
	}
	return (free_env(&envp), rl_clear_history(), 0);
}
