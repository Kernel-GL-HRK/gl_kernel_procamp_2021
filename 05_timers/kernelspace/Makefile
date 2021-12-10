KERNELDIR ?= /work/trainings/linux/buildroot/out/build/linux-5.15.1/

obj-m += custom_timer.o

.PHONY: clean check

all: check
	make -C ${KERNELDIR} M=$(PWD) modules
clean:
	make -C ${KERNELDIR} M=$(PWD) clean
check:
	${KERNELDIR}scripts/checkpatch.pl -f ./custom_timer.c

