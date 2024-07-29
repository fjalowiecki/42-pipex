/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:55:01 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/29 10:38:29 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "../libft/libft.h"

# define ERR_NOCMD "Error: command not found"
# define ERR_TOOFEWARG "Error: Not enough arguments provided by user"
# define ERR_FDOPEN "Error: creating file descriptor failed"
# define ERR_CRTPIPE "Error: pipe creation failed"
# define ERR_DUP2 "Error: file descriptor duplication (dup2) failed"
# define ERR_FORK "Error: fork creation failed"

typedef struct t_data
{
	pid_t	pid1;
	pid_t	pid2;
	int		fd_src;
	int		fd_dest;
	int		fd_pipe[2];
	char	**args_cmd1;
	char	**args_cmd2;
}	t_data;

char	*find_path(char **envp, char *cmd);
void	close_pipes(t_data *data);
void	msg_error_and_exit(char *error);
void	free_ft_split(char **split);
void	open_files(t_data *data, char **argv);
void	child_process_1(t_data *data, char **envp);
void	child_process_2(t_data *data, char **envp);

#endif