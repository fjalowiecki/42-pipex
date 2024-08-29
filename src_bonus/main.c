/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:13:52 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/29 10:56:11 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

/**
 * @brief Entry point for command processing with input validation and piping.
 *
 * Initializes data, validates commands, and manages input/output files based 
 * on arguments. Handles "here_doc" mode or regular file redirection.
 *
 * @param argc The argument count.
 * @param argv The argument vector (array of strings).
 * @param envp The environment variables.
 * @return Status code of the program execution.
 */
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
