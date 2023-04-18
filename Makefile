CFLAGS	=	-Wall -Wextra -Werror -g3

NAME	=	philo

SRC		=	utils3.c\
			utils.c	\
			utils2.c	\
			philosophers.c

SRCS	=	$(SRC:%.c=%.o)

all : $(NAME)

$(NAME) : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -lpthread -o $(NAME)
clean:
	rm -fr $(SRCS)

fclean: clean
	rm -fr $(NAME)

re: fclean all