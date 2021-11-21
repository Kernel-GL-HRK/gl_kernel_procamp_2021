CC=gcc
CFLAGS=-I.
CRYPTOLAGS=-lcrypto
OUT_NAME=rock-paper-scissors

all: $(OUT_NAME)

$(OUT_NAME): rock_paper_scissors.o main.o
	$(CC) rock_paper_scissors.o main.o -o $(OUT_NAME) $(CFLAGS) $(CRYPTOLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

rock_paper_scissors.o: rock_paper_scissors.c
	$(CC) -c rock_paper_scissors.c $(CFLAGS)

clean:
	/bin/rm -rf *.o a.out $(OUT_NAME)
