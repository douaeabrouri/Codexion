# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/23 15:42:38 by doabrour          #+#    #+#              #
#    Updated: 2026/08/23 15:42:39 by doabrour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS =  main.c init.c parsing.c scheduler.c managing_time.c monitor.c routine.c simulation_situation.c heap.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)vv

fclean: clean
	@rm -f $(NAME)

re: fclean all

test: all
	@python3 tests/run_tests.py

.PHONY: all clean fclean re test
