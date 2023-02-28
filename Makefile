# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: miandrad <miandrad@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/27 14:37:38 by miandrad          #+#    #+#              #
#    Updated: 2023/02/27 15:37:20 by miandrad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = cc

RM = rm -rf

CFLAGS = -Wall -Wextra -Werror

LIB = pipex.a

SRC = pipex.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

bonus:	$(BONUS)

$(NAME): $(addprefix SRC/,$(OBJ))
	@make -s -C ft_printf
	@ar rcs $(LIB) $(addprefix SRC/,$(OBJ))
	@$(CC) $(LIB) ft_printf/libftprintf.a -o $(NAME)

run: $(NAME)
	@./pipex abcdfghijklmnopqrstuvwxyz

valgrind: $(NAME)
	@valgrind --leak-check=full ./pipex

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