REALTIME=1
BOOTTIME=2
PROCESS_CPUTIME_ID=3
THREAD_CPUTIME_ID=4


all: userspace_time.c
	gcc userspace_time.c -o userspace_time

test: all
	./timers $(REALTIME)
	./timers $(BOOTTIME)
	./timers $(PROCESS_CPUTIME_ID)
	./timers $(THREAD_CPUTIME_ID)
