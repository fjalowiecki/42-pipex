/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pb_find_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:27:31 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/29 10:56:28 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*parse_cmd(char *cmd)
{
	char	*parsed_cmd;
	size_t	parsed_cmd_len;

	parsed_cmd_len = ft_strlen(cmd) - ft_strlen(ft_strchr(cmd, ' ') + 1);
	parsed_cmd = malloc(sizeof(char) * parsed_cmd_len);
	ft_strlcpy(parsed_cmd, cmd, parsed_cmd_len);
	return (parsed_cmd);
}

static char	*find_final_path(char **envp, char *parsed_cmd)
{
	char	**envp_paths;
	char	*final_envp_path;
	char	*envp_path_part;
	int		i;

	envp_path_part = ft_strjoin("/", parsed_cmd);
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
	return (final_envp_path);
}

char	*find_path(char **envp, char *cmd)
{
	char	*final_envp_path;
	char	*parsed_cmd;

	if (ft_strchr(cmd, ' ') != NULL)
		parsed_cmd = parse_cmd(cmd);
	else
		parsed_cmd = cmd;
	final_envp_path = NULL;
	while (ft_strncmp(*envp, "PATH", 4) != 0)
		envp++;
	final_envp_path = find_final_path(envp, parsed_cmd);
	if (parsed_cmd != cmd)
		free(parsed_cmd);
	return (final_envp_path);
}
