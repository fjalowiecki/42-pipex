/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:50:15 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/19 09:58:54 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char **envp, char *cmd)
{
	char	**envp_paths;
	char	*final_envp_path;
	char	*envp_path_part;
	int		i;

	final_envp_path = NULL;
	envp_path_part = ft_strjoin("/", cmd);
	while (ft_strncmp(*envp, "PATH", 4) != 0)
		envp++;
	envp_paths = ft_split(*envp + 5, ':');
	i = 0;
	while (envp_paths[i] != NULL)
	{
		final_envp_path = ft_strjoin(envp_paths[i], envp_path_part);
		if (access(final_envp_path, F_OK) == 0)
			break ;
		free(final_envp_path);
		i++;
	}
	free_ft_split(envp_paths);
	free(envp_path_part);
	return (final_envp_path);
}

void	close_pipes(t_data *data)
{
	close(data->fd_pipe[0]);
	close(data->fd_pipe[1]);
}

void	msg_error_and_exit(char *error)
{
	perror(error);
	exit(EXIT_FAILURE);
}

void	free_ft_split(char **split)
{
	char	**orig_split;

	orig_split = split;
	while (*split != NULL)
	{
		free(*split);
		split++;
	}
	free(orig_split);
}
