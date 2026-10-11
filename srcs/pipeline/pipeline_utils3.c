/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:04:32 by lkiloul           #+#    #+#             */
/*   Updated: 2025/06/24 17:04:32 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	cleanup_and_exit(t_minishell *minishell)
{
	if (minishell->pipeline->pipes)
		close_pipes(minishell->pipeline->pipes,
			minishell->pipeline->cmd_count);
	exit(1);
}

void	execute_pipeline_command(t_command *cmd,
			int cmd_index, t_minishell *minishell)
{
	t_token					*token;
	int						saved_fds[2];
	t_redirection_params	redir_params;

	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	setup_command_io(minishell->pipeline, cmd_index, saved_fds);
	redir_params.token = cmd->redirections;
	redir_params.envp = &minishell->env;
	redir_params.token_lexer = minishell->tokens;
	redir_params.pipeline = minishell->pipeline;
	redir_params.state = &minishell->state;
	if (cmd->redirections && handle_redirections(&redir_params) == -1)
		restore_fds_and_exit(saved_fds, minishell);
	close(saved_fds[0]);
	close(saved_fds[1]);
	token = create_command_token(cmd);
	if (!token)
		cleanup_and_exit(minishell);
	handle_builtin_execution(cmd, minishell, token);
	free_pipeline(minishell->pipeline);
	free_cmd(cmd);
}

int	process_child_wait(pid_t pid, int *last_status)
{
	int	status;

	if (waitpid(pid, &status, WUNTRACED) > 0)
	{
		if (WIFSTOPPED(status))
		{
			printf("\n[%d]+  Stopped\t\tcommand\n", pid);
			*last_status = 0;
		}
		else
			*last_status = handle_child_status(status);
	}
	return (0);
}

void	init_minishell_struct(t_init_context *ctx)
{
	ctx->minishell->env = *(ctx->envp);
	ctx->minishell->pipeline = ctx->pipeline;
	ctx->minishell->tokens = ctx->token_lexer;
	if (ctx->state)
		ctx->minishell->state = *(ctx->state);
}

int	execute_pipeline_children(t_pipeline *pipeline, pid_t *pids,
			t_minishell *minishell)
{
	int	i;

	i = 0;
	while (i < pipeline->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return (handle_fork_error(pipeline, pids, i));
		if (pids[i] == 0)
		{
			free(pids);
			execute_pipeline_command(pipeline->commands[i], i, minishell);
		}
		i++;
	}
	return (0);
}
