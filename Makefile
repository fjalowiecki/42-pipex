# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjalowie <fjalowie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/29 10:36:34 by fjalowie          #+#    #+#              #
#    Updated: 2024/07/30 12:19:42 by fjalowie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= pipex

SRCS	= src/pipex.c src/utils.c
SRCS_BONUS	= src_bonus/pipex_bonus.c \
				src_bonus/pb_pipeline.c \
				src_bonus/pb_utils.c \
				src_bonus/get_next_line.c \

OBJS	= $(SRCS:.c=.o)
OBJS_BONUS	= $(SRCS_BONUS:.c=.o)

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I includes/

all:	$(NAME)

$(NAME): $(OBJS)
	@$(MAKE) --no-print-directory -C ./libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) libft/libft.a
	@echo "Build of $(NAME) completed."

bonus: $(OBJS_BONUS)
	@$(MAKE) --no-print-directory -C ./libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS_BONUS) libft/libft.a
	@echo "Build of $(NAME) with bonus completed."

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@$(MAKE) --no-print-directory clean -C ./libft
	@rm -f $(OBJS) $(OBJS_BONUS)
	@echo "Clean of ${NAME} completed."

fclean: clean
	@$(MAKE) --no-print-directory fclean -C ./libft
	@rm -f $(NAME)
	@echo "Full clean of ${NAME} completed."

re: fclean all
	@echo "Rebuild of ${NAME} completed."

.PHONY: all clean fclean re bonus