CC = gcc
CLFAGS = -g -Wall

.PHONY: clean all


all: clean memory_tester


main.o:
	$(CC) -c $(CLFAGS)  main.c -o main.o

memory_tester: main.o
	$(CC) main.o -o memory_tester

clean:
	rm -rf memory_tester
	rm -rf main.o