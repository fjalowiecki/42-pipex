/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/29 12:43:51 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	duplicate_fds(t_data *data, int input_fd, int output_fd)
{
	if (dup2(input_fd, STDIN_FILENO) < 0)
		msg_error_and_exit(data, ERR_DUP12);
	if (dup2(output_fd, STDOUT_FILENO) < 0)
		msg_error_and_exit(data, ERR_DUP2);
}

static void get_cmd(t_data *data, int argc_cnt)
{
	char	*origin_cmd;

	data->cmd = ft_split(data->argv[argc_cnt - 1], ' ');
	if (access(data->cmd[0], F_OK) != 0)
	{
		origin_cmd = data->cmd[0];
		data->cmd[0] = find_path(data->envp, data->cmd[0]);
		free(origin_cmd);
	}
}

static void process_last_cmd(t_data *data, int input_fd)
{
	if (data->fd_dest <= -1)
		msg_error_and_exit(data, ERR_FDOPEN);
	duplicate_fds(data, input_fd, data->fd_dest);
	close(input_fd);
	close(data->fd_dest);
	data->fd_dest = -1;
	if (execve(data->cmd[0], data->cmd, data->envp) < 0)
		msg_error_and_exit(data, ERR_EXECVE);
}

static void process_cmd(t_data *data, int input_fd, int *fd_pipe)
{
	duplicate_fds(data, input_fd, fd_pipe[1]);
	close(input_fd);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	if (execve(data->cmd[0], data->cmd, data->envp) < 0)
		msg_error_and_exit(data, ERR_EXECVE);
}

void	recursive_pipeline(int input_fd, t_data *data, int argc_cnt)
{
	int		fd_pipe[2];
	pid_t	pid;

	get_cmd(data, argc_cnt);
	if (argc_cnt == data->argc - 1)
		process_last_cmd(data, input_fd);
	else
	{	
		pipe(fd_pipe);
		pid = fork();
		if (pid < 0)
			msg_error_and_exit(data, ERR_FORK);
		else if (pid == 0)
			process_cmd(data, input_fd, fd_pipe);
		else
		{
			close(fd_pipe[1]);
			waitpid(pid, NULL, 0);
			close(input_fd);
			input_fd = -1;
			free_cmd(data);
			recursive_pipeline(fd_pipe[0], data, argc_cnt + 1);
		}
	}
}

int get_shell_input(t_data *data)
{
	int fd_pipe[2];
	char	*line;

	pipe(fd_pipe);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, data->argv[2], ft_strlen(data->argv[2])) == 0)
			break;
		write(fd_pipe[1], line, ft_strlen(line));
	}
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}

int main(int argc, char *argv[], char *envp[])
{
	t_data data;

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
