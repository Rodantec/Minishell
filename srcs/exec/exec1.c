/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodantec <rodantec@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:04:16 by rodantec          #+#    #+#             */
/*   Updated: 2025/06/23 12:15:00 by rodantec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run_child_process(t_exec_context *ctx)
{
	int						redirection_count;
	t_redirection_params	redir_params;

	setup_child_signals();
	redirection_count = contains_redirection(ctx->token);
	if (redirection_count > 0)
	{
		redir_params.token = ctx->token;
		redir_params.envp = ctx->env;
		redir_params.token_lexer = ctx->token;
		redir_params.pipeline = NULL;
		redir_params.state = ctx->state;
		if (handle_redirections(&redir_params) < 0)
		{
			free_pipeline(ctx->pipeline);
			free_cmd(ctx->cmd);
			exit(1);
		}
	}
	execute_cmd(ctx->token, ctx->env, ctx->state);
	perror("Error");
	exit(1);
}

static void	process_child_status(int status, pid_t pid, t_shell_state *state)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			state->exit_status = 130;
		else if (WTERMSIG(status) == SIGQUIT)
			state->exit_status = 131;
	}
	else if (WIFSTOPPED(status))
	{
		printf("\n[%d]+  Stopped\t\t%s\n", pid, "command");
		state->exit_status = 0;
	}
	else
		state->exit_status = WEXITSTATUS(status);
}

void	first_child(t_exec_context *ctx)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(1);
	}
	if (pid == 0)
	{
		run_child_process(ctx);
	}
	ctx->state->child_pid = pid;
	ctx->state->shell_status = 1;
	setup_execution_signals();
	waitpid(pid, &status, WUNTRACED);
	setup_interactive_signals();
	ctx->state->child_pid = 0;
	ctx->state->shell_status = 0;
	process_child_status(status, pid, ctx->state);
}

char	*check_absolute_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (ft_strdup(cmd));
		}
		return (NULL);
	}
	return (NULL);
}
