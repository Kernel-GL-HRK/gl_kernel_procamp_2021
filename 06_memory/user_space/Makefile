CALL_ALLOCATOR=1
CALL_FREE=1

CC=gcc
CFLAGS=-I. -std=c99
CONDFAGS=-DCALL_ALLOCATOR=$(CALL_ALLOCATOR) -DCALL_FREE=$(CALL_FREE)
OUT_NAME=user-space-memory

all: $(OUT_NAME)

$(OUT_NAME): user_space_memory.c
	$(CC) user_space_memory.c -o $(OUT_NAME) $(CFLAGS) $(CONDFAGS)

clean:
	/bin/rm -rf *.o a.out $(OUT_NAME)
