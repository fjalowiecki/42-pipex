/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:55:01 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/29 11:06:03 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <stdbool.h>
# include "../libft/libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# define ERR_INV_CMD "Error: invalid command"
# define ERR_TOOFEWARG "Error: wrong number of arguments provided"
# define ERR_FDOPEN "Error: invalid input file"
# define ERR_EXECVE "Internal error: execve failed"
# define ERR_CRTPIPE "Internal error: pipe creation failed"
# define ERR_DUP2 "Internal error: file descriptor duplication (dup2) failed"
# define ERR_FORK "Internal error: fork creation failed"
# define ERR_WRONG_PARAM "Internal error: wrong parameter"

typedef struct s_data
{
	int		argc;
	char	**argv;
	char	**envp;
	int		fd_src;
	int		fd_dest;
	char	**cmd;
	bool	no_output_file;
}	t_data;

/* pb_inout_functions.c */

/**
 * @brief Get shell input, create pipe and return pipe fd.
 * @param data Pointer to t_data structure.
 * @return int Reading pipe file descriptor.
 */
int		get_shell_input(t_data *data);
/**
 * @brief Opens the destination file with specified write mode.
 * @param data Pointer to the data structure.
 * @param append_param 0 to truncate, 1 to append.
 */
void	open_dest_file(t_data *data, int append_param);
/**
 * @brief Opens the source file for reading.
*
 * Opens the file at the last argument position for writing, either truncating 
 * or appending based on the append_param. Exits on failure.
 *
 * @param data Pointer to the data structure.
 */
void	open_src_file(t_data *data);

/* pb_validation.c */

/**
 * @brief Checks if the last argument is a valid output file or a command.
 * @param data Pointer to the data structure.
 */
void	check_if_output_is_defined(t_data *data);

/**
 * @brief Validates each command argument to ensure it is executable.
 *
 * Checks each command argument for printability and verifies that the command 
 * path is executable. Exits on invalid command or path issues.
 *
 * @param data Pointer to the data structure.
 */
void	validate_commands(t_data *data);

/* pb_find_path.c */

/**
 * @brief Find the path of a command.
 * @param envp Environment variables.
 * @param cmd Command to find.
 * @return char* Path of the command.
 */
char	*find_path(char **envp, char *cmd);

/** pb_pipeline.c **/

/**
 * @brief Executes commands in a recursive pipeline fashion.
 *
 * Sets up and executes a pipeline of commands, handling input/output redirection
 * and recursion for multiple commands. Handles the last command separately.
 *
 * @param input_fd File descriptor for the input of the current command.
 * @param data Pointer to the data structure.
 * @param argc_cnt Index of the current command argument being processed.
 */
void	recursive_pipeline(int input_fd, t_data *data, int argc_cnt);

/** pb_utils.c **/

/**
 * @brief Display an error message, free memory and exit.
 * @param data Pointer to t_data structure.
 * @param error Error message to display.
 */
void	msg_error_and_exit(t_data *data, char *error);
/**
 * @brief Free the command array.
 * @param data Pointer to t_data structure.
 */
void	free_cmd(t_data *data);
/**
 * @brief Free a split string array.
 * @param split Split string array to free.
 */
void	free_ft_split(char **split);

/** get_next_line.c **/

/**
 * @brief Get the next line from a file descriptor.
 * @param fd File descriptor to read from.
 * @return char* Line read from the file descriptor.
 */
char	*get_next_line(int fd);

#endif