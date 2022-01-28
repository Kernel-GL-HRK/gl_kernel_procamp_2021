CC=gcc
CC_FLAGS=-std=gnu11 -Wall

CC_SOURCES=memtime.c
TARGET_NAME=memtime

$(TARGET_NAME):
	$(CC) $(CC_FLAGS) -o $@ $(CC_SOURCES) -lm

all: clean $(TARGET_NAME)

clean:
	-$(RM) -rf *.o $(TARGET_NAME)

