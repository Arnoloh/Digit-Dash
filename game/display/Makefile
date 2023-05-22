#Makefile
CC = gcc -g
CPPFLAGS = -fsanitize=address -lncurses
CFLAGS = -Wall -Wextra -std=c99


SRC = ${wildcard */*.c *.c} ../../Chat/chat.c ../find_word/markov.c ../../Client/customer.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: test

-include ${DEP}

test: ${OBJ}
	$(CC) ${CFLAGS} ${CPPFLAGS} $^ -o $@
	

.PHONY: all clean

clean:
	${RM} -r  ${OBJ} ${DEP} test
