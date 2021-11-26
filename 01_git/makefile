#
# makefile of scissors-paper game project
#

HEADERS = scissors.h
OBJECTS = scissors.o

scissors: ${OBJECTS}
		gcc -o scissors ${OBJECTS}

scissors.o: scissors.c $(HEADERS)
		gcc -c scissors.c

.PHONY: clean

clean:
		rm $(OBJECTS)
