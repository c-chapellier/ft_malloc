SRCS =	src/malloc.c \
		src/realloc.c \
		src/free.c \
		src/show_alloc_mem.c \
		src/show_alloc_mem_ex.c

OBJS = ${SRCS:.c=.o}

ifeq ($(HOSTTYPE),)
HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_${HOSTTYPE}.so

CFLAGS = -Wall -Wextra -Werror

all :		${NAME}

${NAME} :	${OBJS}
			gcc -shared -o ${NAME} ${OBJS} -lm
			ln -s -f ${NAME} libft_malloc.so

clean :
			rm -f ${OBJS}

fclean :	clean
			rm -f ${NAME} libft_malloc.so

re :		fclean all
