CFLAGS	=	-g3 # -fsanitize=thread

NAME	=	philo

SRC		=	utils3.c\
			utils.c	\
			utils4.c	\
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