# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/27 14:37:38 by miandrad          #+#    #+#              #
#    Updated: 2023/03/07 12:52:43 by miandrad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = cc

RM = rm -rf

CFLAGS = -Wall -Wextra -Werror

LIB = pipex.a

SRC = pipex.c

GET_SRC = get_next_line.c get_next_line_utils.c

OBJ = $(SRC:.c=.o)

GET_OBJ = $(GET_SRC:.c=.o)

all: $(NAME)

$(NAME): $(addprefix SRC/,$(OBJ)) $(addprefix get_next_line_100/,$(GET_OBJ))
	@make -s -C ft_printf
	@ar rcs $(LIB) $(addprefix SRC/,$(OBJ)) $(addprefix get_next_line_100/,$(GET_OBJ))
	@$(CC) $(LIB) ft_printf/libftprintf.a -o $(NAME)

run: $(NAME)
	@./pipex aa "cat" "cat" bb

valgrind: $(NAME)
	@valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./pipex aa "grep a1" "wc -w" bb

git: fclean
	@git add .
	@git commit
	@git push
	@echo " -------Commited and Pushed------- "

clean: 
	@make clean -s -C ft_printf
	@$(RM) $(addprefix SRC/,$(OBJ))

fclean: clean
	@make fclean -s -C ft_printf
	@$(RM) $(NAME) $(LIB)

re: fclean all