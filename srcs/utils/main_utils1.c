/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:41:52 by lkiloul           #+#    #+#             */
/*   Updated: 2025/06/24 17:41:52 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*prompt_and_read(t_shell_state *state)
{
	char	*line;

	g_signal_received = 0;
	line = readline("minishell> ");
	if (!line)
	{
		if (g_signal_received == SIGINT)
		{
			g_signal_received = 0;
			return (ft_strdup(""));
		}
		if (state->shell_status == 0)
		{
			printf("exit\n");
			exit(state->exit_status);
		}
		return (NULL);
	}
	if (line && *line)
		add_history(line);
	return (line);
}

int	find_command_token(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
			return (1);
		current = current->next;
	}
	return (0);
}

void	execute_command_logic(t_command_logic_context *ctx)
{
	if (is_builtin(ctx->cmd->args[0]))
	{
		if (contains_redirection(ctx->tokens) > 0)
		{
			setup_child_signals();
			first_child(ctx->ctx);
			setup_interactive_signals();
		}
		else
			handle_builtin_cmd(ctx->tokens, ctx->cmd, ctx->envp, ctx->state);
	}
	else
	{
		setup_child_signals();
		first_child(ctx->ctx);
		setup_interactive_signals();
	}
}
