/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pb_pipeline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:51:11 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/28 19:59:54 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Duplicate file descriptors for input and output.
 * @param data Pointer to t_data structure.
 * @param input_fd File descriptor for input.
 * @param output_fd File descriptor for output.
 */
static void	duplicate_fds(t_data *data, int input_fd, int output_fd)
{
	if (dup2(input_fd, STDIN_FILENO) < 0)
		msg_error_and_exit(data, ERR_DUP2);
	if (dup2(output_fd, STDOUT_FILENO) < 0)
		msg_error_and_exit(data, ERR_DUP2);
}

/**
 * @brief Get command from arguments and verify its path.
 * @param data Pointer to t_data structure.
 * @param argc_cnt Argument count.
 */
static void	get_cmd(t_data *data, int argc_cnt)
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

/**
 * @brief Process the last command in the pipeline.
 * @param data Pointer to t_data structure.
 * @param input_fd File descriptor for input.
 */
static void	process_last_cmd(t_data *data, int input_fd)
{
	if (data->no_output_file)
		duplicate_fds(data, input_fd, STDOUT_FILENO);
	else
		duplicate_fds(data, input_fd, data->fd_dest);
	close(input_fd);
	if (data->fd_dest != -1)
		close(data->fd_dest);
	data->fd_dest = -1;
	if (data->cmd[0] != NULL)
	{
		if (execve(data->cmd[0], data->cmd, data->envp) < 0)
			msg_error_and_exit(data, ERR_EXECVE);
	}
	msg_error_and_exit(data, ERR_EXECVE);
}

/**
 * @brief Process a command in the pipeline.
 * @param data Pointer to t_data structure.
 * @param input_fd File descriptor for input.
 * @param fd_pipe Pipe file descriptors.
 */
static void	process_cmd(t_data *data, int input_fd, int *fd_pipe)
{
	duplicate_fds(data, input_fd, fd_pipe[1]);
	close(input_fd);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	if (data->cmd[0] != NULL)
	{
		if (execve(data->cmd[0], data->cmd, data->envp) < 0)
			msg_error_and_exit(data, ERR_EXECVE);
	}
	msg_error_and_exit(data, ERR_EXECVE);
}

/**
 * @brief Create a recursive pipeline.
 * @param input_fd File descriptor for input.
 * @param data Pointer to t_data structure.
 * @param argc_cnt Argument count.
 */
void	recursive_pipeline(int input_fd, t_data *data, int argc_cnt)
{
	int		fd_pipe[2];
	pid_t	pid;

	get_cmd(data, argc_cnt);
	if ((data->no_output_file == false && argc_cnt == data->argc - 1)
		|| (data->no_output_file == true && argc_cnt == data->argc))
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
