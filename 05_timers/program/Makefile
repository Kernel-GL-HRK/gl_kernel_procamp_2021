CC = ../../../buildroot-2021.02.7/BUILD/host/bin/x86_64-buildroot-linux-uclibc-gcc

OBJS = my_clock.o

all:	my_clock

my_clock:	$(OBJS)
	$(CC) -o $@ $^

clean:
	rm -fr Makefile_Prj $(OBJS)
