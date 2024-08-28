/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pb_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:50:15 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/28 18:44:08 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*parse_cmd(char *cmd)
{
	char *parsed_cmd;
	size_t	parsed_cmd_len;

	parsed_cmd_len = ft_strlen(cmd) - ft_strlen(ft_strchr(cmd, ' ') + 1);
	parsed_cmd = malloc(sizeof(char) * parsed_cmd_len);
	ft_strlcpy(parsed_cmd, cmd, parsed_cmd_len);
	return (parsed_cmd);
}

/**
 * @brief Find the path of a command.
 * @param envp Environment variables.
 * @param cmd Command to find.
 * @return char* Path of the command.
 */
char	*find_path(char **envp, char *cmd)
{
	char	**envp_paths;
	char	*final_envp_path;
	char	*envp_path_part;
	char	*parsed_cmd = NULL;
	int		i;

	if (ft_strchr(cmd, ' ') != NULL)
		parsed_cmd = parse_cmd(cmd);
	else
		parsed_cmd = cmd;
	final_envp_path = NULL;
	envp_path_part = ft_strjoin("/", parsed_cmd);
	while (ft_strncmp(*envp, "PATH", 4) != 0)
		envp++;
	envp_paths = ft_split(*envp + 5, ':');
	i = 0;
	while (envp_paths[i] != NULL)
	{
		final_envp_path = ft_strjoin(envp_paths[i], envp_path_part);
		if (access(final_envp_path, X_OK) == 0)
			break ;
		free(final_envp_path);
		final_envp_path = NULL;
		i++;
	}
	free_ft_split(envp_paths);
	free(envp_path_part);
	if (parsed_cmd != cmd)
		free(parsed_cmd);
	return (final_envp_path);
}

/**
 * @brief Display an error message, free memory and exit.
 * @param data Pointer to t_data structure.
 * @param error Error message to display.
 */
void	msg_error_and_exit(t_data *data, char *error)
{
	if (data->fd_src != -1)
		close(data->fd_src);
	if (data->fd_dest != -1)
		close(data->fd_dest);
	if (data->cmd != NULL)
		free_cmd(data);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

/**
 * @brief Free the command array.
 * @param data Pointer to t_data structure.
 */
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

/**
 * @brief Free a split string array.
 * @param split Split string array to free.
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
