#Makefile
CC = gcc -g
CPPFLAGS = -fsanitize=address
CFLAGS+= -I/path/to/parent/directory/of/tools -Wall -Werror -g -fsanitize=address


SRC = ${wildcard *.c}
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

-include ${DEP}

main: ${OBJ}
	$(CC) ${CFLAGS} ${CPPFLAGS} $^  -o $@
	

.PHONY:all clean

clean:
	${RM} -r  ${OBJ} ${DEP} main
