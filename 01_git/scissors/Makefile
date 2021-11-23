TARGET = rps_game

CC = gcc

CFLAGS = -Wall -lcrypto -lssl

.PHONY: all clean

all: $(TARGET)
	@echo "build rps_game"
	
$(TARGET): main.c
	@$(CC)  main.c $(CFLAGS) -Wl,-Map,$(TARGET).map -o $@

clean:
	@rm -f $(TARGET) *.map *.asm

