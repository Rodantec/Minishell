/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodantec <rodantec@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:28:11 by rodantec          #+#    #+#             */
/*   Updated: 2025/06/23 11:55:00 by rodantec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>

int	process_special_redirections(char *input, int *i, t_token **tokens)
{
	t_token	*token;
	char	c;

	c = input[*i];
	if (input[*i + 1] == c)
	{
		if (c == '<')
			token = create_token(TOKEN_HEREDOC, "<<");
		else
			token = create_token(TOKEN_APPEND, ">>");
		if (!token)
			return (-1);
		add_token(tokens, token);
		(*i) += 2;
		return (1);
	}
	return (0);
}

int	check_word_end(char *input, int i, int in_quotes)
{
	if (!in_quotes)
	{
		if (is_separator(input[i]) || input[i] == '|'
			|| input[i] == '<' || input[i] == '>')
			return (1);
	}
	return (0);
}

void	handle_quotes(char c, int *in_quotes, char *quote_type)
{
	if ((c == '\'' || c == '\"') && (!*in_quotes || c == *quote_type))
	{
		*in_quotes = !*in_quotes;
		if (*in_quotes)
			*quote_type = c;
		else
			*quote_type = 0;
	}
}

int	add_eof_token(t_token **tokens)
{
	t_token	*token;

	token = create_token(TOKEN_EOF, "");
	if (!token)
		return (-1);
	add_token(tokens, token);
	return (0);
}

t_token	*lexer(char *input, t_env *envp, t_shell_state *state)
{
	t_token				*tokens;
	int					i;
	t_process_context	ctx;

	tokens = NULL;
	i = 0;
	ctx.input = input;
	ctx.i = &i;
	ctx.tokens = &tokens;
	ctx.envp = envp;
	ctx.state = state;
	while (input[i])
	{
		if (process_lexer_token(input, &i, &tokens, &ctx) == -1)
			return (free_tokens(&tokens), NULL);
	}
	if (add_eof_token(&tokens) == -1)
		return (free_tokens(&tokens), NULL);
	return (tokens);
}
