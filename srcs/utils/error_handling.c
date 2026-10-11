/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:40 by lkiloul           #+#    #+#             */
/*   Updated: 2025/07/09 10:52:40 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_command_not_found(char *cmd)
{
	if (cmd)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
}

void	print_syntax_error(void)
{
	ft_putstr_fd("minishell: syntax error\n", 2);
}

void	print_syntax_error_token(char *token, t_shell_state *state)
{
	if (token)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("'\n", 2);
	}
	else
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `newline'\n", 2);
	}
	if (state)
		state->exit_status = 2;
}

int	check_redirection_error(t_token *current, t_shell_state *state)
{
	if ((is_redirection(current))
		&& (!current->next || current->next->type != TOKEN_WORD))
	{
		if (current->next && current->next->type != TOKEN_EOF)
			print_syntax_error_token(current->next->value, state);
		else
			print_syntax_error_token(NULL, state);
		return (-1);
	}
	return (0);
}

int	check_pipe_error(t_token *current, t_shell_state *state)
{
	if (current->type == TOKEN_PIPE)
	{
		if (!current->next || current->next->type == TOKEN_EOF)
		{
			print_syntax_error_token(NULL, state);
			return (-1);
		}
		if (current->next->type == TOKEN_PIPE)
		{
			print_syntax_error_token("|", state);
			return (-1);
		}
	}
	return (0);
}
