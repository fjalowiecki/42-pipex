/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pb_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 16:50:15 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/29 11:07:11 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
