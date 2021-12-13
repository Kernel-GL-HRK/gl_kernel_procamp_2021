KERNELDIR ?= ../output/build/linux-5.10.7

obj-m := mem_alloc_mod.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
