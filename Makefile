#Makefile
CC = gcc -g
CPPFLAGS = -fsanitize=address -lncurses
CFLAGS = -Wall -Wextra -std=c99


SRC = ${wildcard *.c game/display/*.c } Chat/chat.c game/find_word/markov.c  Client/customer.c game/display/menu/ui.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

-include ${DEP}

main: ${OBJ}
	$(CC) ${CFLAGS} ${CPPFLAGS} $^ -o $@
	

.PHONY: all clean

clean:
	${RM} -r  ${OBJ} ${DEP} main
