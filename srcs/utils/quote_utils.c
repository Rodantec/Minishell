/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:29:55 by lkiloul           #+#    #+#             */
/*   Updated: 2025/06/24 18:29:55 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_variable_replacement(t_variable_replacement_context *ctx)
{
	char	*result;

	result = process_dollar_sign(ctx->current, ctx->i, ctx->envp, ctx->state);
	if (!result)
		return (NULL);
	if (result != ctx->current && ctx->current != ctx->temp_word)
		free(ctx->current);
	return (result);
}

int	process_character(char *current, int i, char *quote_type, int *in_quotes)
{
	update_quote_state(current[i], quote_type, in_quotes);
	if (current[i] == '$' && *quote_type != '\'')
		return (1);
	return (0);
}

void	init_scan_context(t_variable_replacement_context *ctx,
			t_context_init_params *params)
{
	ctx->current = params->current;
	ctx->temp_word = params->temp_word;
	ctx->i = params->i;
	ctx->envp = params->envp;
	ctx->state = params->state;
}

char	*process_variable_in_loop(t_context_init_params *params)
{
	t_variable_replacement_context	ctx;

	init_scan_context(&ctx, params);
	return (handle_variable_processing(&ctx));
}

char	*scan_and_expand(t_scan_expand_params *scan_params)
{
	int						i;
	int						in_quotes;
	char					quote_type;
	char					*result;
	t_context_init_params	params;

	i = scan_params->start_i;
	in_quotes = 0;
	quote_type = 0;
	while (scan_params->current[i])
	{
		if (process_character(scan_params->current, i, &quote_type, &in_quotes))
		{
			setup_params(&params, scan_params, i);
			result = process_variable_in_loop(&params);
			if (!result)
				return (NULL);
			if (result != scan_params->current)
				return (result);
		}
		i++;
	}
	return (scan_params->current);
}
