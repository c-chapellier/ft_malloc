SRCS =	src/malloc.c \
		src/realloc.c \
		src/free.c \
		src/show_alloc_mem.c \
		src/show_alloc_mem_hex.c \
		src/show_alloc_mem_hist.c

OBJS = ${SRCS:.c=.o}

ifeq ($(HOSTTYPE),)
HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_${HOSTTYPE}.so

CFLAGS = -Wall -Wextra -Werror

TEST_FILE = test0

all :		${NAME}

${NAME} :	${OBJS}
			gcc -shared -o ${NAME} ${OBJS} -lm
			ln -s -f ${NAME} libft_malloc.so
	
test :		
			gcc -o ${TEST_FILE} ${TEST_FILE}.c -D REAL=1
			/usr/bin/time -l ./${TEST_FILE}
			rm ${TEST_FILE}

my_test :		
			gcc -o ${TEST_FILE} ${TEST_FILE}.c libft_malloc.so
			/usr/bin/time -l ./${TEST_FILE}
			rm ${TEST_FILE}

clean :
			rm -f ${OBJS}

fclean :	clean
			rm -f ${NAME} libft_malloc.so

re :		fclean all
