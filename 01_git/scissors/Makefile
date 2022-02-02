# Makefile for Rock-Paper-Scissors game
# Author: ViacheslavHolubnychyi<viacheslav.holubnychyi@globallogic.com>

C=gcc
OBJS=RockPaperScissors.o

program: ${OBJS}
	${C} -o program ${OBJS}

RockPaperScissors.o: RockPaperScissors.c
	${C} -c RockPaperScissors.c

clean:
	rm *.o program
