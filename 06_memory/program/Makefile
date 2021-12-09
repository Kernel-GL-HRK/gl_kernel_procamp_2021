CC = ../../../buildroot-2021.02.7/BUILD/host/bin/x86_64-buildroot-linux-uclibc-gcc

OBJS = allocate_test.o

all:	allocate_test

allocate_test:	$(OBJS)
	$(CC) -o $@ $^ -lm

clean:
	rm -fr allocate_test $(OBJS)
