/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/28 19:56:07 by fjalowie         ###   ########.fr       */
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

static void	open_dest_file(t_data *data, int append_param)
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

static void	open_src_file(t_data *data)
{
	data->fd_src = open(data->argv[1], O_RDONLY);
	if (data->fd_src <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
}

static void	check_if_output_is_defined(t_data *data)
{
	char	*cmd_path;

	cmd_path = find_path(data->envp, data->argv[data->argc - 1]);
	if (cmd_path != NULL)
	{
		data->no_output_file = true;
		free(cmd_path);
	}
	else
		data->no_output_file = false;
}

static void	validate_commands(t_data *data)
{
	char *cmd_path;
	int	i;
	int j;

	if (ft_strncmp(data->argv[1], "here_doc", 9) == 0)
		i = 3;
	else
		i = 2;
	while (data->argv[i] != NULL
		&& (data->no_output_file == false && i < data->argc - 1))
	{
		if (data->argv[i][0] == '\0')
			msg_error_and_exit(data, ERR_INV_CMD);
		j = 0;
		while (data->argv[i][j] != '\0')
		{
			if (ft_isprint(data->argv[i][j]) == 0)
				printf("isprint %c\n", data->argv[i][j]);
			j++;
		}
		cmd_path = find_path(data->envp, data->argv[i]);
		if (access(cmd_path, X_OK != 0))
		{
			free(cmd_path);
			msg_error_and_exit(data, ERR_INV_CMD);
		}
		free(cmd_path);
		i++;
	}
} 

void static	initialize_data(t_data *data, int argc, char *argv[], char *envp[])
{
	data->argc = argc;
	data->argv = argv;
	data->envp = envp;
	data->cmd = NULL;
	data->fd_dest = -1;
	data->fd_src = -1;
	data->no_output_file = false;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;

	initialize_data(&data, argc, argv, envp);
	check_if_output_is_defined(&data);
	validate_commands(&data);
	if (ft_strncmp(data.argv[1], "here_doc", 9) == 0)
	{
		if (argc < 4)
			msg_error_and_exit(&data, ERR_TOOFEWARG);
		if (data.no_output_file == false)
			open_dest_file(&data, 1);
		recursive_pipeline(get_shell_input(&data), &data, 4);
	}
	else
	{
		if (argc < 3)
			msg_error_and_exit(&data, ERR_TOOFEWARG);
		open_src_file(&data);
		if (data.no_output_file == false)
			open_dest_file(&data, 0);
		recursive_pipeline(data.fd_src, &data, 3);
	}
}
