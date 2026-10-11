/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodantec <rodantec@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 10:58:48 by rodantec          #+#    #+#             */
/*   Updated: 2025/05/21 12:07:52 by rodantec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open <");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("dup2 <");
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_output_append(char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open >>");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("dup2 >>");
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_output_trunc(char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open >");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("dup2 >");
		return (-1);
	}
	close(fd);
	return (0);
}

int	contains_redirection(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		if (is_redirection(current))
			count++;
		current = current->next;
	}
	return (count);
}

int	process_redirections(t_redirection_params *params,
		int stdin_backup, int stdout_backup)
{
	t_token					*current;
	t_redirection_params	single_params;

	current = params->token;
	while (current)
	{
		if (is_redirection(current))
		{
			single_params.token = current;
			single_params.envp = params->envp;
			single_params.token_lexer = params->token_lexer;
			single_params.pipeline = params->pipeline;
			single_params.state = params->state;
			if (apply_single_redirection(&single_params) == -1)
				return (restore_file_descriptors(stdin_backup, stdout_backup),
					-1);
			current = current->next;
		}
		else
			current = current->next;
	}
	return (0);
}
