/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/30 12:01:24 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Executes the first child process in the pipeline.
 * 
 * @param data Pointer to the data structure containing command and file 
 * descriptors.
 * @param envp Array of environment variables.
 */
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
		msg_error_and_exit(data, ERR_DUP2);
	if (dup2(data->fd_pipe[1], STDOUT_FILENO) < 0)
		msg_error_and_exit(data, ERR_DUP2);
	close(data->fd_pipe[0]);
	data->fd_pipe[0] = -1;
	if (data->args_cmd1[0] != NULL)
		execve(data->args_cmd1[0], data->args_cmd1, envp);
	msg_error_and_exit(data, ERR_NOCMD);
}

/**
 * @brief Executes the second child process in the pipeline.
 * 
 * @param data Pointer to the data structure containing command and file 
 * descriptors.
 * @param envp Array of environment variables.
 */
void	child_process_2(t_data *data, char **envp)
{
	char	*origin_args_cmd2;

	if (access(data->args_cmd2[0], F_OK) != 0)
	{
		origin_args_cmd2 = data->args_cmd2[0];
		data->args_cmd2[0] = find_path(envp, data->args_cmd2[0]);
		free(origin_args_cmd2);
	}
	if (dup2(data->fd_pipe[0], STDIN_FILENO) < 0)
		msg_error_and_exit(data, ERR_DUP2);
	close(data->fd_pipe[1]);
	data->fd_pipe[1] = -1;
	if (dup2(data->fd_dest, STDOUT_FILENO) < 0)
		msg_error_and_exit(data, ERR_DUP2);
	if (data->args_cmd2[0] != NULL)
		execve(data->args_cmd2[0], data->args_cmd2, envp);
	msg_error_and_exit(data, ERR_NOCMD);
}

/**
 * @brief Initializes the data structure with default values.
 * 
 * @param data Pointer to the data structure to initialize.
 */
void	initialize_data(t_data *data)
{
	data->args_cmd1 = NULL;
	data->args_cmd2 = NULL;
	data->fd_src = -1;
	data->fd_dest = -1;
	data->fd_pipe[0] = -1;
	data->fd_pipe[1] = -1;
}

/**
 * @brief Main function to execute the pipex program.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param envp Array of environment variables.
 * @return int Exit status of the program.
 */
int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;

	initialize_data (&data);
	if (argc != 5)
		msg_error_and_exit(&data, ERR_TOOFEWARG);
	open_files (&data, argv);
	data.args_cmd1 = ft_split(argv[2], ' ');
	data.args_cmd2 = ft_split(argv[3], ' ');
	if (pipe(data.fd_pipe) <= -1)
		msg_error_and_exit(&data, ERR_CRTPIPE);
	data.pid1 = fork();
	if (data.pid1 < 0)
		msg_error_and_exit(&data, ERR_FORK);
	else if (data.pid1 == 0)
		child_process_1(&data, envp);
	data.pid2 = fork();
	if (data.pid2 < 0)
		msg_error_and_exit(&data, ERR_FORK);
	else if (data.pid2 == 0)
		child_process_2(&data, envp);
	close_pipes(&data);
	waitpid(data.pid1, NULL, 0);
	waitpid(data.pid2, NULL, 0);
	free_ft_split(data.args_cmd1);
	free_ft_split(data.args_cmd2);
}
