/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodantec <rodantec@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:10:41 by rodantec          #+#    #+#             */
/*   Updated: 2025/06/23 11:15:00 by rodantec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_pipeline_execution(t_pipeline *pipeline, pid_t **pids)
{
	if (!pipeline || !pipeline->commands || pipeline->cmd_count < 1)
		return (-1);
	if (pipeline->cmd_count > 1)
	{
		pipeline->pipes = create_pipes(pipeline->cmd_count);
		if (!pipeline->pipes)
			return (-1);
	}
	*pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!*pids)
	{
		if (pipeline->pipes)
			close_pipes(pipeline->pipes, pipeline->cmd_count);
		return (-1);
	}
	return (0);
}

int	handle_fork_error(t_pipeline *pipeline, pid_t *pids, int i)
{
	while (--i >= 0)
		waitpid(pids[i], NULL, WUNTRACED);
	free(pids);
	if (pipeline->pipes)
		close_pipes(pipeline->pipes, pipeline->cmd_count);
	perror("fork");
	return (-1);
}

int	handle_child_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

int	wait_for_children(t_pipeline *pipeline, pid_t *pids, t_shell_state *state)
{
	int	i;
	int	last_status;

	i = 0;
	last_status = 0;
	if (state)
		state->shell_status = 1;
	setup_execution_signals();
	while (i < pipeline->cmd_count)
	{
		process_child_wait(pids[i], &last_status);
		i++;
	}
	setup_interactive_signals();
	if (state)
		state->shell_status = 0;
	return (free(pids), last_status);
}

int	execute_pipeline(t_pipeline *pipeline, t_env *envp,
		t_token *token_lexer, t_shell_state *state)
{
	pid_t			*pids;
	t_minishell		minishell;
	t_init_context	ctx;
	int				result;

	if (init_pipeline_execution(pipeline, &pids) == -1)
		return (-1);
	ctx.minishell = &minishell;
	ctx.pipeline = pipeline;
	ctx.envp = envp;
	ctx.token_lexer = token_lexer;
	ctx.state = state;
	init_minishell_struct(&ctx);
	result = execute_pipeline_children(pipeline, pids, &minishell);
	if (result != 0)
		return (result);
	if (pipeline->pipes)
		close_pipes(pipeline->pipes, pipeline->cmd_count);
	return (pipeline->pipes = NULL, wait_for_children(pipeline, pids, state));
}
