/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:34:09 by fjalowie          #+#    #+#             */
/*   Updated: 2024/05/14 12:41:57 by fjalowie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char * argv[])
{
    char buffer[5];
    char * filename = argv[1];
    int fd;
    
    if (access(filename, R_OK) != 0)
        printf("file not accessible to read"); //need to change to errno
    fd = open (filename, O_RDONLY);
    read(fd, buffer, 5);
    // printf("output: %s", filename);
    printf("output: %s", buffer);
}