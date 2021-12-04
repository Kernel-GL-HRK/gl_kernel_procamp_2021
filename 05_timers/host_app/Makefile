CC = gcc
CLFAGS = -g -Wall

.PHONY: clean all


all: clean sysclock_app


main.o:
	$(CC) -c $(CLFAGS)  main.c -o main.o

sysclock_app: main.o
	$(CC) main.o -o sysclock_app

clean:
	rm -rf sysclock_app
	rm -rf main.o