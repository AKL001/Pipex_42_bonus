PROG = pipex

PROG_B = pipex_bonus

SRCS_B = bonus/pipex_bonus.c\
		 bonus/herdoc_utils.c\
		 bonus/handler_bonus.c\
		 bonus/executor_bonus.c\
		 bonus/error_utils_bonus.c\
		 bonus/cleanups.c\

OBJS_B = $(SRCS_B:.c=.o)

SRCS = mandatory/pipex.c\
	   mandatory/executor.c\
	   mandatory/error_utils.c\
	   

OBJS = $(SRCS:.c=.o)

HEADER = -I.

DEPS = pipex.h

CC = cc

CFLAGS = -Wall -Wextra -Werror -fsanitize=address 
# -fsanitize=address

all: $(PROG)

$(PROG): $(OBJS)
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJS) -Llibft -lft -o $(PROG)

bonus: $(PROG_B)

$(PROG_B): $(OBJS_B)
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJS_B) -Llibft -lft -o $(PROG_B)

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

clean:
	@make clean -C libft
	@rm -f $(OBJS) $(OBJS_B)

fclean: clean
	@make fclean -C libft
	@rm -f $(PROG)

fclean_bonus: clean
	@rm -f $(PROG_B)

re: fclean all

re_bonus: fclean bonus

.PHONY: all clean fclean re re_bonus bonus
