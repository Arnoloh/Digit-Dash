#Makefile

CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra -g -fsanitize=address
CPPFLAGS = -g -fsanitize=address
LDFLAGS = -fsanitize=address
LDLIBS = `pkg-config --libs glib-2.0`

all: customer

customer: main.o customer.o ../Chat/chat.o
main.o: main.c customer.h ../Chat/chat.h
customer.o: customer.c customer.h ../Chat/chat.c ../Chat/chat.o

.PHONY: clean

clean:
	${RM} customer main.o customer.o ../Chat/chat.o

#END
