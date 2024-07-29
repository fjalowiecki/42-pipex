/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:55:01 by fjalowie          #+#    #+#             */
/*   Updated: 2024/07/29 12:36:50 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <stdbool.h>
# include "../libft/libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# define ERR_EXECVE "Error: execve failed"
# define ERR_TOOFEWARG "Error: Not enough arguments provided by user"
# define ERR_FDOPEN "Error: creating file descriptor failed"
# define ERR_CRTPIPE "Error: pipe creation failed"
# define ERR_DUP2 "Error: file descriptor duplication (dup2) failed"
# define ERR_DUP12 "Error: input file descriptor duplication (dup2) failed"
# define ERR_FORK "Error: fork creation failed"

typedef struct s_data
{
	int		argc;
	char	**argv;
	char	**envp;
	int		fd_src;
	int		fd_dest;
	char	**cmd;
	bool	here_doc;
} t_data;


char	*find_path(char **envp, char *cmd);
void	msg_error_and_exit(t_data *data, char *error);
void	free_cmd(t_data *data);
void	open_src_and_dest_files(t_data *data);
void	open_dest_file(t_data *data);
void	free_ft_split(char **split);
char	*get_next_line(int fd);

#endif