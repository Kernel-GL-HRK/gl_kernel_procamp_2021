#
# makefile of memory_tst application
#

OBJECTS = memory_tst.o

get_time: ${OBJECTS}
		gcc -o memory_tst ${OBJECTS}

get_time.o: memory_tst.c $(HEADERS)
		gcc -c memory_tst.c

.PHONY: clean

clean:
		rm $(OBJECTS)
