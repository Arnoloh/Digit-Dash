CC = gcc -g
CPPFLAGS = -fsanitize=address 
CFLAGS+= -Wall -Werror -g -fsanitize=address
LDLIBS = -lncurses

SRC = main.c chat.c ../tools/tools.c ../Client/customer.c ../game/display/player.c ../game/find_word/markov.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

-include ${DEP}

main: ${OBJ}
	$(CC) ${CFLAGS} ${CPPFLAGS} $^ -o $@ $(LDLIBS)

.PHONY:all clean

clean:
	${RM} -r  ${OBJ} ${DEP} main
