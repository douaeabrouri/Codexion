NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = init.c parsing.c scheduler.c managing_time.c monitor.c routine.c simulation_situation.c routine.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) codexion

fclean: clean
	@rm -f $(NAME)

re: fclean all

test: all
	@python3 tests/run_tests.py

.PHONY: all clean fclean re test
