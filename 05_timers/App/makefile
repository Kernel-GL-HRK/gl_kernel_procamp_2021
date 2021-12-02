#
# makefile of get_time application
#

OBJECTS = get_time.o

get_time: ${OBJECTS}
		gcc -o get_time ${OBJECTS}

get_time.o: get_time.c $(HEADERS)
		gcc -c get_time.c

.PHONY: clean

clean:
		rm $(OBJECTS)
