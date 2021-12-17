CC ? = gcc
SOURCES = user_space.c
EXECUTABLE = get_UTC_time
OBJECTS = $(SOURCES:.c=.o)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $< -c -o $@
