/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:33:57 by lkiloul           #+#    #+#             */
/*   Updated: 2025/06/24 18:33:57 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_var_end(char *str, int start)
{
	int	j;

	j = start + 1;
	if (str[j] == '?')
		return (j + 1);
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	return (j);
}

char	*create_prefix_and_cleanup(char *temp_word, int i, char *var_name)
{
	char	*prefix;

	prefix = ft_substr(temp_word, 0, i);
	if (!prefix)
		free(var_name);
	return (prefix);
}

char	*expand_loop(char *result, t_env *envp, t_shell_state *state)
{
	char	*temp;
	char	*old_result;

	while (ft_strchr(result, '$'))
	{
		old_result = result;
		temp = expand_once(result, envp, state);
		if (!temp || ft_strcmp(result, temp) == 0)
		{
			free(temp);
			break ;
		}
		result = temp;
		free(old_result);
	}
	return (result);
}

char	*process_variable_expansion(t_variable_replacement_context *ctx)
{
	char	*result;

	result = handle_variable_replacement(ctx);
	if (!result)
		return (NULL);
	if (result != ctx->current)
		return (result);
	return (ctx->current);
}

char	*expand_variables(char *temp_word, t_env *envp, t_shell_state *state)
{
	char					*current;
	char					*result;
	int						i;
	t_scan_expand_params	params;

	current = temp_word;
	i = 0;
	while (1)
	{
		params.current = current;
		params.temp_word = temp_word;
		params.start_i = i;
		params.envp = envp;
		params.state = state;
		result = scan_and_expand(&params);
		if (!result)
			return (NULL);
		if (result == current)
			break ;
		current = result;
	}
	return (current);
}
