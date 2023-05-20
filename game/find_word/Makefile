# Makefile

CC = gcc -g
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = markov.c list.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

markov: ${OBJ}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} markov

# END
