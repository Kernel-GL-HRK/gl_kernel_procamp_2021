KERNELDIR ?= ../output/build/maklinux-5.10.7/ 

obj-m := par_module.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) par_module.c

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
