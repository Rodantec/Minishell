/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodantec <rodantec@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:17:33 by rodantec          #+#    #+#             */
/*   Updated: 2025/06/23 11:50:00 by rodantec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static char	*generate_heredoc_basename(void)
{
	char	*base;

	base = ft_strdup("/tmp/.heredoc_0");
	return (base);
}

char	*create_heredoc_filename(void)
{
	char	*filename;

	filename = generate_heredoc_basename();
	if (!filename)
		return (NULL);
	return (filename);
}

int	is_delimiter(char *line, char *delimiter)
{
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static void	process_and_write_line(t_heredoc_content_params *params, char *line)
{
	char	*expanded_line;

	if (params->should_expand)
	{
		expanded_line = expand_variables(line, params->envp, params->state);
		if (expanded_line)
		{
			ft_putendl_fd(expanded_line, params->fd);
			if (expanded_line != line)
				free(expanded_line);
		}
		else
			ft_putendl_fd(line, params->fd);
	}
	else
		ft_putendl_fd(line, params->fd);
}

void	write_heredoc_content(t_heredoc_content_params *params)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("\nwarning: here-document delimited by end-of-file\n",
				2);
			break ;
		}
		if (is_delimiter(line, params->delimiter))
			break ;
		process_and_write_line(params, line);
		free(line);
	}
}
