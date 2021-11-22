
CC=gcc
CFLAGS="-c"


.PHONY:  all clean


all: scissors

clean:
	rm scissors
	rm *.o


scissors: main.o rpsEngine.o
	${CC} -o scissors main.o rpsEngine.o




main.o: main.c 
	gcc -c main.c

rpsEngine.o: rpsEngine.c rpsEngine.h
	gcc -c rpsEngine.c
