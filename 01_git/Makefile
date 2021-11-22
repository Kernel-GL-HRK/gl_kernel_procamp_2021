C=gcc
LDFLAGS=-lcrypto

RM=rm -rf

all: rock_scissors_paper.o main.o rsp_game

rsp_game: rock_scissors_paper.o main.c
	$(C) rock_scissors_paper.o main.c -o rsp_game $(LDFLAGS)

main.o: main.c
	$(C) -c main.c

rock_scissors_paper.o: rock_scissors_paper.c rock_scissors_paper.h
	$(C) -c rock_scissors_paper.c rock_scissors_paper.h

clean:
	$(RM) *.o *.h.gch rsp_game
