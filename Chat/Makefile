#Makefile
CC = gcc -g
CPPFLAGS = -fsanitize=address
CFLAGS+= -Wall -Werror -g -fsanitize=address


SRC = main.c chat.c ../tools/tools.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

-include ${DEP}

main: ${OBJ}
	$(CC) ${CFLAGS} ${CPPFLAGS} $^  -o $@
	

.PHONY:all clean

clean:
	${RM} -r  ${OBJ} ${DEP} main
