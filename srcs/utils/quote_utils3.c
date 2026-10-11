/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:08:08 by lkiloul           #+#    #+#             */
/*   Updated: 2025/07/15 19:30:48 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_params(t_context_init_params *params,
			t_scan_expand_params *scan_params, int i)
{
	params->current = scan_params->current;
	params->temp_word = scan_params->temp_word;
	params->i = i;
	params->envp = scan_params->envp;
	params->state = scan_params->state;
}

char	*handle_variable_processing(t_variable_replacement_context *ctx)
{
	char	*result;

	result = process_variable_expansion(ctx);
	if (!result)
		return (NULL);
	if (result != ctx->current)
		return (result);
	return (ctx->current);
}

char	*process_quotes_and_expand(char *word, t_env *envp,
			t_shell_state *state)
{
	char	*temp_word;
	char	*expanded;
	char	*result;

	if (!word)
		return (NULL);
	temp_word = ft_strdup(word);
	if (!temp_word)
		return (NULL);
	expanded = expand_variables(temp_word, envp, state);
	if (expanded != temp_word)
		free(temp_word);
	if (!expanded)
		return (NULL);
	result = remove_quotes(expanded);
	free(expanded);
	return (result);
}
