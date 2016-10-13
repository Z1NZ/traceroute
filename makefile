CC    = gcc  -Wall -Wextra -Werror -g 
# -fsanitize=address

LDFLAGS	= -I./includes/ -I./libft/includes -lncurses

RM    = rm -f

NAME    = ft_traceroute

SRC    = main.c\
		traceroute.c\
		ft_in_cksum.c\
		ft_error.c\

OBJS    = $(SRC:.c=.o)
SRCDIR 	= ./srcs/
SRCS	= $(addprefix $(SRCDIR), $(SRC))
all : $(NAME)

$(NAME):
	@make -C libft/
	@$(CC) $(SRCS) $(LDFLAGS) -L./libft -lft -o $(NAME) 
clean:
	@$(RM) $(OBJS)
	@make clean -C libft/ 
fclean: clean
	@$(RM) $(NAME)
	@make fclean -C libft/
re: fclean all

.PHONY: all clean fclean re