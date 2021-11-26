CC=gcc
CC_FLAGS=-std=c99 -Wall

CC_SOURCES=main.c
TARGET_NAME=rock_paper_scissors

$(TARGET_NAME):
	$(CC) $(CC_FLAGS) -o $@ $(CC_SOURCES)

all: clean $(TARGET_NAME)

clean:
	-$(RM) -rf *.o $(TARGET_NAME)

