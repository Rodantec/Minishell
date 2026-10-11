/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkiloul <lkiloul@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:52:31 by lkiloul           #+#    #+#             */
/*   Updated: 2025/07/09 10:52:31 by lkiloul          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**get_path_cut(t_env *env)
{
	int		i;
	char	**pathcut;

	i = 0;
	while (env->env[i] && ft_strnstr(env->env[i], "PATH=", 5) == NULL)
		i++;
	if (!env->env[i])
		return (NULL);
	pathcut = ft_split(env->env[i] + 5, ':');
	return (pathcut);
}
