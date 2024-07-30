/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/30 14:50:56 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Get shell input, create pipe and return pipe fd.
 * @param data Pointer to t_data structure.
 * @return int Reading pipe file descriptor.
 */
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

static void	open_dest_file(t_data *data)
{
	data->fd_dest = open(data->argv[data->argc - 1],
			O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (data->fd_dest <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
}

static void	open_src_and_dest_files(t_data *data)
{
	data->fd_src = open(data->argv[1], O_RDONLY);
	if (data->fd_src <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
	data->fd_dest = open(data->argv[data->argc - 1],
			O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (data->fd_dest <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;

	data.argc = argc;
	data.argv = argv;
	data.envp = envp;
	data.cmd = NULL;
	data.fd_dest = -1;
	data.fd_src = -1;
	if (argc < 4)
	{
		perror(ERR_TOOFEWARG);
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(data.argv[1], "here_doc", 9) == 0)
	{
		open_dest_file(&data);
		recursive_pipeline(get_shell_input(&data), &data, 4);
	}
	else
	{
		open_src_and_dest_files(&data);
		recursive_pipeline(data.fd_src, &data, 3);
	}
}
