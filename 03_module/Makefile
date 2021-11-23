KERNELDIR ?= /work/trainings/linux/buildroot/out/build/linux-5.15.1/

obj-m += custom_printk.o
all:
	make -C ${KERNELDIR} M=$(PWD) modules
clean:
	make -C ${KERNELDIR} M=$(PWD) clean
