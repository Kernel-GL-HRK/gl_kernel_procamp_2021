CC=gcc
SOURCES=main.c
EXECUTABLE=scissors
OBJECTS=$(SOURCES:.c=.o)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $< -c -o $@

clean:
	rm -f *.o $(EXECUTABLE)
