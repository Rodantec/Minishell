/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:59:32 by lkiloul           #+#    #+#             */
/*   Updated: 2025/06/24 16:59:32 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_heredoc(t_heredoc_params *params)
{
	char						*filename;
	int							fd;
	t_heredoc_content_params	content_params;

	filename = create_heredoc_filename();
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(filename);
		return (NULL);
	}
	content_params.fd = fd;
	content_params.delimiter = params->delimiter;
	content_params.should_expand = params->should_expand;
	content_params.envp = params->envp;
	content_params.state = params->state;
	write_heredoc_content(&content_params);
	close(fd);
	return (filename);
}

void	cleanup_heredoc(char *filename)
{
	if (filename)
	{
		unlink(filename);
		free(filename);
	}
}
