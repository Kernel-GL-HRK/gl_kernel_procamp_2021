#https://makefiletutorial.com/#makefile-cookbook
CC=gcc
CFLAGS= -g -Wall

.PHONY: clean

all: clean scissors

clean:
	-$(RM) -rf *.o
scissors:
	$(CC) $(CFLAGS) -o scissors_game main.c
