/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:52:36 by lkiloul           #+#    #+#             */
/*   Updated: 2025/06/24 16:52:36 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_redirect(t_token *token, t_env *envp, t_shell_state *state)
{
	char				*clean;
	char				*filename;
	int					expand;
	int					result;
	t_heredoc_params	heredoc_params;

	expand = !is_quoted_delimiter(token->next->value);
	clean = remove_quotes_from_delimiter(token->next->value);
	heredoc_params.delimiter = clean;
	heredoc_params.should_expand = expand;
	heredoc_params.envp = envp;
	heredoc_params.state = state;
	filename = handle_heredoc(&heredoc_params);
	free(clean);
	if (!filename)
		return (-1);
	result = redirect_input(filename);
	cleanup_heredoc(filename);
	return (result);
}

int	apply_single_redirection(t_redirection_params *params)
{
	(void)params->pipeline;
	if (!params->token || !params->token->next
		|| params->token->next->type != TOKEN_WORD)
		return (-1);
	if (params->token->type == TOKEN_REDIR_OUT)
		return (redirect_output_trunc(params->token->next->value));
	if (params->token->type == TOKEN_APPEND)
		return (redirect_output_append(params->token->next->value));
	if (params->token->type == TOKEN_REDIR_IN)
	{
		if (redirect_input(params->token->next->value) < 0)
		{
			if (params->token_lexer)
				free_tokens(&params->token_lexer);
			if (params->envp)
				free_env(params->envp);
			return (-1);
		}
		return (0);
	}
	if (params->token->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirect(params->token,
				params->envp, params->state));
	return (0);
}

t_token	*extract_redirections(t_token *tokens)
{
	t_token	*head;
	t_token	*cur;
	t_token	*redir;
	t_token	*file;

	head = NULL;
	cur = tokens;
	while (cur)
	{
		if (is_redirection(cur) && cur->next && cur->next->type == TOKEN_WORD)
		{
			redir = create_token(cur->type, cur->value);
			file = create_token(cur->next->type, cur->next->value);
			if (!redir || !file)
				return (NULL);
			redir->next = file;
			head = redir;
			cur = cur->next;
		}
		cur = cur->next;
	}
	return (head);
}

int	handle_redirections(t_redirection_params *params)
{
	int	stdin_backup;
	int	stdout_backup;

	if (!params->token)
		return (0);
	if (save_file_descriptors(&stdin_backup, &stdout_backup) == -1)
		return (-1);
	if (process_redirections(params, stdin_backup, stdout_backup) == -1)
		return (-1);
	close(stdin_backup);
	close(stdout_backup);
	return (0);
}
