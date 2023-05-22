#Makefile
CC = gcc -g -w
CPPFLAGS = -fsanitize=address -lncurses
CFLAGS = -Wall -Wextra -std=c99


SRC = ${wildcard *.c game/display/*.c } Chat/chat.c game/find_word/markov.c  Client/customer.c game/display/menu/ui.c  game/display/help/help.c main.c tools/tools.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: digit-dash

-include ${DEP}

digit-dash: ${OBJ}
	$(CC) ${CFLAGS} ${CPPFLAGS} $^ -o $@
	

.PHONY: all clean

clean:
	${RM} -r  ${OBJ} ${DEP} main
