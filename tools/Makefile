
CC = gcc -g
CFLAGS = -Wall -Wextra
LDLIBS = 

EXE = tools

all: $(EXE)

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} -rf $(EXE) *.dSYM
