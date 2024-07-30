/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:50:15 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/30 12:03:03 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Finds the path of a command.
 * 
 * @param envp Array of environment variables.
 * @param cmd Command to find the path for.
 * @return char* Path of the command.
 */
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
		final_envp_path = NULL;
		i++;
	}
	free_ft_split(envp_paths);
	free(envp_path_part);
	return (final_envp_path);
}

/**
 * @brief Opens the source and destination files.
 * 
 * @param data Pointer to the data structure containing file descriptors.
 * @param argv Argument vector containing file paths.
 */
void	open_files(t_data *data, char **argv)
{
	data->fd_src = open(argv[1], O_RDONLY);
	if (data->fd_src <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
	data->fd_dest = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (data->fd_dest <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
}

/**
 * @brief Closes the pipe file descriptors.
 * 
 * @param data Pointer to the data structure containing pipe file descriptors.
 */
void	close_pipes(t_data *data)
{
	close(data->fd_pipe[0]);
	data->fd_pipe[0] = -1;
	close(data->fd_pipe[1]);
	data->fd_pipe[1] = -1;
}

/**
 * @brief Prints an error message and exits the program.
 * 
 * @param data Pointer to the data structure containing command and file 
 * descriptors.
 * @param msg Error message to print.
 */
void	msg_error_and_exit(t_data *data, char *msg)
{
	if (data->args_cmd1 != NULL)
		free_ft_split(data->args_cmd1);
	if (data->args_cmd2 != NULL)
		free_ft_split(data->args_cmd2);
	if (data->fd_src != -1)
		close(data->fd_src);
	if (data->fd_dest != -1)
		close(data->fd_dest);
	if (data->fd_pipe[0] != -1)
		close(data->fd_pipe[0]);
	if (data->fd_pipe[1] != -1)
		close(data->fd_pipe[1]);
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * @brief Frees a null-terminated array of strings.
 * 
 * @param split Array of strings to free.
 */
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
