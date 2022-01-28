CC=gcc
CC_FLAGS=-std=gnu11 -Wall

CC_SOURCES=timegetter.c
TARGET_NAME=timegetter

$(TARGET_NAME):
	$(CC) $(CC_FLAGS) -o $@ $(CC_SOURCES)

all: clean $(TARGET_NAME)

clean:
	-$(RM) -rf *.o $(TARGET_NAME)
