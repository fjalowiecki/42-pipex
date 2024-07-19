# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/13 14:20:27 by fjalowie          #+#    #+#              #
#    Updated: 2024/07/19 13:43:33 by fjalowie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= pipex

SRCS	= pipex.c utils.c
SRCS_BONUS	= pipex_bonus.c utils_bonus.c

OBJS	= $(SRCS:.c=.o)
OBJS_BONUS	= $(SRCS_BONUS:.c=.o)

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

all:	$(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C ./libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) libft/libft.a

bonus: $(OBJS_BONUS)
	$(MAKE) -C ./libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS_BONUS) libft/libft.a

clean:
	$(MAKE) clean -C ./libft
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	$(MAKE) fclean -C ./libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus

