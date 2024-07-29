/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/29 10:41:12 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process_1(t_data *data, char **envp)
{
	char	*origin_args_cmd1;

	if (access(data->args_cmd1[0], F_OK) != 0)
	{
		origin_args_cmd1 = data->args_cmd1[0];
		data->args_cmd1[0] = find_path(envp, data->args_cmd1[0]);
		free(origin_args_cmd1);
	}
	if (dup2(data->fd_src, STDIN_FILENO) < 0)
		msg_error_and_exit(ERR_DUP2);
	if (dup2(data->fd_pipe[1], STDOUT_FILENO) < 0)
		msg_error_and_exit(ERR_DUP2);
	close(data->fd_pipe[0]);
	if (data->args_cmd1[0] != NULL)
		execve(data->args_cmd1[0], data->args_cmd1, envp);
	msg_error_and_exit(ERR_NOCMD);
}

void	child_process_2(t_data *data, char **envp)
{
	char	*origin_args_cmd2;

	if (access(data->args_cmd2[0], F_OK) != 0)
	{
		origin_args_cmd2 = data->args_cmd2[0];
		data->args_cmd2[0] = find_path(envp, data->args_cmd2[0]);
		free(origin_args_cmd2);
	}
	printf("data->args_cmd2[0]: %s\n", data->args_cmd2[0]);
	if (dup2(data->fd_pipe[0], STDIN_FILENO) < 0)
		msg_error_and_exit(ERR_DUP2);
	close(data->fd_pipe[1]);
	if (dup2(data->fd_dest, STDOUT_FILENO) < 0)
		msg_error_and_exit(ERR_DUP2);
	if (data->args_cmd2[0] != NULL)
		execve(data->args_cmd2[0], data->args_cmd2, envp);
	msg_error_and_exit(ERR_NOCMD);
}

void	open_files(t_data *data, char **argv)
{
	data->fd_src = open(argv[1], O_RDONLY);
	if (data->fd_src <= -1)
		msg_error_and_exit(ERR_FDOPEN);
	data->fd_dest = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (data->fd_dest <= -1)
		msg_error_and_exit(ERR_FDOPEN);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;

	if (argc != 5)
		msg_error_and_exit(ERR_TOOFEWARG);
	open_files (&data, argv);
	if (pipe(data.fd_pipe) <= -1)
		msg_error_and_exit(ERR_CRTPIPE);
	data.args_cmd1 = ft_split(argv[2], ' ');
	data.args_cmd2 = ft_split(argv[3], ' ');
	data.pid1 = fork();
	if (data.pid1 < 0)
		msg_error_and_exit(ERR_FORK);
	else if (data.pid1 == 0)
		child_process_1(&data, envp);
	data.pid2 = fork();
	if (data.pid2 < 0)
		msg_error_and_exit(ERR_FORK);
	else if (data.pid2 == 0)
		child_process_2(&data, envp);
	close_pipes(&data);
	waitpid(data.pid1, NULL, 0);
	waitpid(data.pid2, NULL, 0);
	free_ft_split(data.args_cmd1);
	free_ft_split(data.args_cmd2);
}
