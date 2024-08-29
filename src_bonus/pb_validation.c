/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pb_validation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:28:42 by fjalowie          #+#    #+#             */
/*   Updated: 2024/08/29 10:51:24 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	check_if_output_is_defined(t_data *data)
{
	char	*cmd_path;

	if (data->argv[data->argc - 1][0] == '\0')
		msg_error_and_exit(data, ERR_INV_CMD);
	cmd_path = find_path(data->envp, data->argv[data->argc - 1]);
	if (cmd_path != NULL)
	{
		data->no_output_file = true;
		free(cmd_path);
	}
	else
		data->no_output_file = false;
}

static void	check_if_printable(t_data *data, char *str)
{
	int	i;

	if (str[0] == '\0')
		msg_error_and_exit(data, ERR_INV_CMD);
	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isprint(str[i]) == 0)
			msg_error_and_exit(data, ERR_INV_CMD);
		i++;
	}
}

void	validate_commands(t_data *data)
{
	char	*cmd_path;
	int		i;

	if (ft_strncmp(data->argv[1], "here_doc", 9) == 0)
		i = 3;
	else
		i = 2;
	while (data->argv[i] != NULL
		&& (data->no_output_file == false && i < data->argc - 1))
	{
		check_if_printable(data, data->argv[i]);
		cmd_path = find_path(data->envp, data->argv[i]);
		if (access(cmd_path, X_OK != 0))
		{
			free(cmd_path);
			msg_error_and_exit(data, ERR_INV_CMD);
		}
		free(cmd_path);
		i++;
	}
}
