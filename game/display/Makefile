#Makefile
CC = gcc -g
CPPFLAGS =
CFLAGS = -Wall -Wextra -std=c99


SRC = ${wildcard *.c} ../find_word/markov.c ../../Chat/chat.c
OBJ = ${SRC:.c=.o} 
DEP = ${SRC:.c=.d} 

all: test

-include ${DEP}

test: ${OBJ}
	$(CC) ${CFLAGS} ${CPPFLAGS} $^ -o $@
	

.PHONY: all clean

clean:
	${RM} -r  ${OBJ} ${DEP} test
