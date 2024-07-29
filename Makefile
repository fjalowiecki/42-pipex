# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/29 10:36:34 by fjalowie          #+#    #+#              #
#    Updated: 2024/07/29 10:36:41 by fjalowie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= pipex

SRCS	= src/pipex.c src/utils.c
SRCS_BONUS	= src_bonus/pipex_bonus.c src_bonus/utils_bonus.c src_bonus/get_next_line.c

OBJS	= $(SRCS:.c=.o)
OBJS_BONUS	= $(SRCS_BONUS:.c=.o)

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g -I includes/

all:	$(NAME)

$(NAME): $(OBJS)
	@$(MAKE) -C ./libft
	@mkdir -p bin
	@$(CC) $(CFLAGS) -o bin/$(NAME) $(OBJS) libft/libft.a

bonus: $(OBJS_BONUS)
	@$(MAKE) -C ./libft
	@mkdir -p bin
	@$(CC) $(CFLAGS) -o bin/$(NAME) $(OBJS_BONUS) libft/libft.a

clean:
	@$(MAKE) clean -C ./libft
	@rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	@$(MAKE) fclean -C ./libft
	@rm -f bin/$(NAME)

re: fclean all

.PHONY: all clean fclean re bonus