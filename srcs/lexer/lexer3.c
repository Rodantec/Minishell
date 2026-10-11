/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:38:59 by lkiloul           #+#    #+#             */
/*   Updated: 2025/07/15 20:33:20 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_lexer_token(char *input, int *i, t_token **tokens,
		t_process_context *ctx)
{
	if (is_separator(input[*i]))
	{
		(*i)++;
		return (0);
	}
	else if (input[*i] == '<' || input[*i] == '>')
	{
		if (process_redirection(input, i, tokens) == -1)
			return (-1);
	}
	else if (input[*i] == '|')
	{
		if (process_pipe(input, i, tokens) == -1)
			return (-1);
	}
	else if (process_word(ctx) == -1)
		return (-1);
	return (0);
}
