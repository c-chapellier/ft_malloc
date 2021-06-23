SRCS =	main.c \
		src/ft_malloc.c \
		src/ft_realloc.c \
		src/ft_free.c \
		src/show_alloc_mem.c \
		
OBJS = ${SRCS:.c=.o}

NAME = no_name

CFLAGS		= -Wall -Wextra -Werror

all :		${NAME}

${NAME} :	${OBJS}
			gcc ${OBJS}

clean :
			rm -f ${OBJS}

fclean :	clean
			rm -f a.out

re :		fclean all
