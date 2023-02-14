#Makefile

CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address
CPPFLAGS = -g -fsanitize=address
LDFLAGS = -fsanitize=address

all: main

main: main.o customer.o
main.o: main.c customer.h
customer.o: customer.c customer.h

.PHONY: clean

clean:
	${RM} main main.o customer.o

#END
