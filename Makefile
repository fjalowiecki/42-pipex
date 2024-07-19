# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/13 14:20:27 by fjalowie          #+#    #+#              #
#    Updated: 2024/07/19 10:00:46 by fjalowie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= pipex

SRCS	= pipex.c \
			utils.c

OBJS	= ${SRCS:.c=.o}

CC		= cc

CFLAGS	= -Wall -Wextra -Werror

$(NAME): $(OBJS)
		$(MAKE) -C ./libft
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) libft/libft.a

all:	${NAME}

clean:
	$(MAKE) clean -C ./libft
	rm -f ${OBJS}

fclean: clean
	$(MAKE) fclean -C ./libft
	rm -f ${NAME}

re: fclean all

.PHONY: all, clean, fclean, re