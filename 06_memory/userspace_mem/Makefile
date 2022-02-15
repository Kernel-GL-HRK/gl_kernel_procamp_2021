MALLOC=1
CALLOC=2
ALLOCA=3


all: userspace_mem.c
	gcc userspace_mem.c -o userspace_mem -lm

test: all
	echo 'MALLOC LOG\n' > log.txt
	./userspace_mem $(MALLOC) >> log.txt
	echo '\nCALLOC LOG\n' >> log.txt
	./userspace_mem $(CALLOC) >> log.txt
	echo '\nALLOCA LOG\n' >> log.txt
	./userspace_mem $(ALLOCA) >> log.txt
	cat log.txt
