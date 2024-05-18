/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/05/18 14:32:41 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "libft/libft.h"

#define BUFFER_SIZE 4096

int main (int argc, char * argv[])
{
	char buffer[BUFFER_SIZE];
	char buffer2[BUFFER_SIZE];
	char * filename_src = argv[1];
	char * filename_dest = argv[4];
	int fd_src;
	int fd_dest;
	
	pid_t p;
	pid_t p2;
	int fd_pipe1[2];
	int fd_pipe2[2];

	char **args_cmd1 = ft_split(argv[2], ' ');
	args_cmd1[0] =  ft_strjoin("/usr/bin/", args_cmd1[0]);
	char **args_cmd2 = ft_split(argv[3], ' ');
	args_cmd2[0] =  ft_strjoin("/usr/bin/", args_cmd2[0]);
	printf("str: %s", args_cmd2[2]);
	// char *args_cmd1[] = { "/usr/bin/grep", "a1", NULL };
	// char *args_cmd2[] = { "/usr/bin/wc", "-w", NULL };

    char *env[] = { NULL };
	// if (argc != 3)
	// {
	// 	write(2, "error: arguments lacking", 25);
	// 	exit(EXIT_FAILURE);
	// }
	
	fd_src = open (filename_src, O_RDONLY);
	if (fd_src <= -1)
	{
		perror("fd_src");
		exit(EXIT_FAILURE);
	}

	fd_dest = open(argv[4], O_RDWR | O_CREAT, 0644);
	if (fd_dest <= -1)
	{
		perror("fd_dest");
		close(fd_src);
		exit(EXIT_FAILURE);
	}

	if (pipe(fd_pipe1) < 0)
	{
		perror("fd_pipe1");
		exit(EXIT_FAILURE);
	}

	if (pipe(fd_pipe2) < 0)
	{
		perror("fd_pipe2");
		exit(EXIT_FAILURE);
	}

	p = fork();

	if (p > 0)
	{
		close(fd_pipe2[0]);
		close(fd_pipe2[1]);

		close(fd_pipe1[1]);
		wait(NULL);
		read(fd_pipe1[0], buffer2, BUFFER_SIZE);
		close(fd_pipe1[0]);
		write(fd_dest, buffer2, strlen(buffer2));
	}
	else if (p == 0)
	{
		p2 = fork();
		if (p2 == 0)
		{
			close(fd_pipe2[0]);

			close(fd_pipe1[0]);
			close(fd_pipe1[1]);
			dup2(fd_src, STDIN_FILENO);
			dup2(fd_pipe2[1], STDOUT_FILENO);
			execve(args_cmd1[0], args_cmd1, env);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else if (p2 > 0)
		{
			close(fd_pipe1[0]);
			close(fd_pipe2[1]);
			wait(NULL);
			dup2(fd_pipe2[0], STDIN_FILENO);
			dup2(fd_pipe1[1], STDOUT_FILENO);
			close(fd_pipe2[0]);
			close(fd_pipe1[1]);
			execve(args_cmd2[0], args_cmd2, env);
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
}