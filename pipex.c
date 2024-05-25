/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/05/25 10:16:17 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	msg_error_and_exit(char *error)
{
	perror(error);
	exit(EXIT_FAILURE);
}

void free_ft_split(char **split)
{
	char	**orig_split = split;

	while(*split != NULL)
	{
		free(*split);
		split++;
	}
	free(orig_split);
}

char	*find_path(char **envp, char *cmd)
{
	char	**envp_paths;
	char	*temp_envp_paths;
	char	*envp_path_part;
	char	*final_path;
	int		i;

	if (!envp || !cmd)
		msg_error_and_exit(ERR_NOARG);
	final_path = NULL;
	envp_path_part = ft_strjoin("/", cmd);
	while (ft_strncmp(*envp, "PATH", 4) != 0)
		envp++;
	envp_paths = ft_split(*envp+5, ':');
	i = 0;
	while(envp_paths[i] != NULL)
	{
		temp_envp_paths = ft_strjoin(envp_paths[i], envp_path_part);
		free(envp_paths[i]);
		envp_paths[i] = temp_envp_paths;
		if (access(envp_paths[i], F_OK) == 0)
		{
			final_path = strdup(envp_paths[i]);
			break;
		}
		i++;
	}
	free_ft_split(envp_paths);
	free(envp_path_part);
	return (final_path);
}

int	child_process(int *fd_pipe, int fd_src, char **args_cmd1, char **envp)
{
	char	*origin_args_cmd1;
	
	if (access(args_cmd1[0], F_OK) != 0)
	{
		origin_args_cmd1 = args_cmd1[0];
		args_cmd1[0] =  find_path(envp, args_cmd1[0]);
		free(origin_args_cmd1);
	}
	close(fd_pipe[0]);
	dup2(fd_src, STDIN_FILENO);
	dup2(fd_pipe[1], STDOUT_FILENO);
	if (args_cmd1[0] != NULL)
		execve(args_cmd1[0], args_cmd1, envp);
	return (-1);
}

int parent_process(int *fd_pipe, int fd_dest, char **args_cmd2, char **envp)
{
	char	*origin_args_cmd2;

	if (access(args_cmd2[0], F_OK) != 0)
	{
		origin_args_cmd2 = args_cmd2[0];
		args_cmd2[0] =  find_path(envp, args_cmd2[0]);
		free(origin_args_cmd2);
	}
	close(fd_pipe[1]);
	wait(NULL);
	dup2(fd_pipe[0], STDIN_FILENO);
	dup2(fd_dest, STDOUT_FILENO);
	if (args_cmd2[0] != NULL)
		execve(args_cmd2[0], args_cmd2, envp);
	return (-1);
}

int	main(int argc, char *argv[], char *envp[])
{
	int fd_src;
	int fd_dest;
	pid_t pid;
	int fd_pipe[2];
	char **args_cmd1;
	char **args_cmd2;

	if (argc != 5)
		msg_error_and_exit(ERR_TOOFEWARG);
	fd_src = open(argv[1], O_RDONLY);
	if (fd_src <= -1)
		msg_error_and_exit(ERR_FDOPEN);
	fd_dest = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_dest <= -1)
		msg_error_and_exit(ERR_FDOPEN); //maybe lacking close(fd_src);
	if (pipe(fd_pipe) <= -1)
		msg_error_and_exit(ERR_CRTPIPE); 
	args_cmd1 = ft_split(argv[2], ' ');
	args_cmd2 = ft_split(argv[3], ' ');
	pid = fork();
	if (pid == 0)
	{
		if (child_process(fd_pipe, fd_src, args_cmd1, envp) == -1)
		{
			free_ft_split(args_cmd1);
			msg_error_and_exit(ERR_NOCMD);
		}
	}
	else if (pid > 0)
	{
		if (parent_process(fd_pipe, fd_dest, args_cmd2, envp) == -1)
		{
			free_ft_split(args_cmd1);
			free_ft_split(args_cmd2);
			msg_error_and_exit(ERR_NOCMD);
		}

	}
}
