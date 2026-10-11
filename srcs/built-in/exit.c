/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodantec <rodantec@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:17:37 by rodantec          #+#    #+#             */
/*   Updated: 2025/05/28 12:15:00 by rodantec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	process_exit_args(char **args, int i, t_shell_state *state)
{
	long	exit_code;

	if (!is_numeric(args[i]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[i], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		state->exit_status = 2;
		state->shell_status = 1;
		return (2);
	}
	if (args[i + 1])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	exit_code = ft_atol(args[i]);
	state->exit_status = (unsigned char)exit_code;
	return (state->exit_status);
}

int	exit_shell(char **args, t_shell_state *state)
{
	int	i;

	i = 1;
	printf("exit\n");
	if (args[i] && ft_strcmp(args[i], "--") == 0)
		i++;
	if (args[i])
		return (process_exit_args(args, i, state));
	else
		state->exit_status = 0;
	state->shell_status = 1;
	return (state->exit_status);
}
