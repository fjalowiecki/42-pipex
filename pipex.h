/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 13:55:01 by fjalowie          #+#    #+#             */
/*   Updated: 2024/05/24 20:22:08 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "libft/libft.h"

#define ERR_NOARG "Error: Funtion argument is null"
#define ERR_NOCMD "Error: command not found"
#define ERR_TOOFEWARG "Error: Not enough arguments provided by user"
#define ERR_FDOPEN "Error: creating file descriptor failed"
#define ERR_CRTPIPE "Error: pipe creation failed"


#endif