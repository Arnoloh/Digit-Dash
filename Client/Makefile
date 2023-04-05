#Makefile

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra -g -fsanitize=address
CPPFLAGS = -g -fsanitize=address
LDFLAGS = -fsanitize=address
LDLIBS = `pkg-config --libs glib-2.0`

all: customer

customer: main.o customer.o
main.o: main.c customer.h
customer.o: customer.c customer.h

.PHONY: clean

clean:
	${RM} customer main.o customer.o

#END
