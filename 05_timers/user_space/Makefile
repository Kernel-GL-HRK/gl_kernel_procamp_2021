CC=gcc
CFLAGS=-I. -std=c99 -D_POSIX_C_SOURCE=199309L
OUT_NAME=user-space-timers

all: $(OUT_NAME)

$(OUT_NAME): user_space_timers.c
	$(CC) user_space_timers.c -o $(OUT_NAME) $(CFLAGS)

clean:
	/bin/rm -rf *.o a.out $(OUT_NAME)
