#https://makefiletutorial.com/#makefile-cookbook
BUILD_DIR=./bin/
CC=gcc
CFLAGS= -g -Wall

.PHONY: all

all: clean scissors

clean:
	-$(RM) -rf *.o
	-$(RM) -rf $(BUILD_DIR)/*
scissors:
	- $(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/scissors_game main.c
