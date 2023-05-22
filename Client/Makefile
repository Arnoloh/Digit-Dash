#Makefile

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra -g -fsanitize=address 
CPPFLAGS = -g -fsanitize=address
LDLIBS = `pkg-config --libs glib-2.0` -lncurses

SRC = main.c customer.c ../Chat/chat.c ../game/display/player.c ../game/find_word/markov.c ../tools/tools.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: customer

-include ${DEP}

customer: ${OBJ}
	$(CC) ${CFLAGS} ${CPPFLAGS} $^  -o $@ $(LDLIBS)

.PHONY: clean

clean:
	${RM} -r  ${OBJ} ${DEP} customer

#END
