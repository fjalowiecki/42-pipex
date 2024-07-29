/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:50:15 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/29 12:41:56 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

void	msg_error_and_exit(t_data *data, char *error)
{
	if (data->fd_src != -1)
		close(data->fd_src);
	if (data->fd_dest != -1)
		close(data->fd_dest);
	if (data->cmd != NULL)
		free_cmd(data);
	perror(error);
	exit(EXIT_FAILURE);
}

void	free_cmd(t_data *data)
{
	char	**cmd_ptr;

	cmd_ptr = data->cmd;
	while (*cmd_ptr != NULL)
	{
		free(*cmd_ptr);
		cmd_ptr++;
	}
	free(data->cmd);
	data->cmd = NULL;
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
void open_dest_file(t_data *data)
{
	data->fd_dest = open(data->argv[data->argc - 1],
			O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (data->fd_dest <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
}

void	open_src_and_dest_files(t_data *data)
{
	data->fd_src = open(data->argv[1], O_RDONLY);
	if (data->fd_src <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
	data->fd_dest = open(data->argv[data->argc - 1],
		O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (data->fd_dest <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
}
