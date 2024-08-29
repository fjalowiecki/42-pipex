/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pb_inout_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/29 10:56:42 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	get_shell_input(t_data *data)
{
	int		fd_pipe[2];
	char	*line;

	pipe(fd_pipe);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, data->argv[2], ft_strlen(data->argv[2])) == 0)
			break ;
		write(fd_pipe[1], line, ft_strlen(line));
	}
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}

void	open_dest_file(t_data *data, int append_param)
{
	if (append_param == 0)
		data->fd_dest = open(data->argv[data->argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else if (append_param == 1)
		data->fd_dest = open(data->argv[data->argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0664);
	else
		msg_error_and_exit(data, ERR_WRONG_PARAM);
	if (data->fd_dest <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
}

void	open_src_file(t_data *data)
{
	data->fd_src = open(data->argv[1], O_RDONLY);
	if (data->fd_src <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
}
