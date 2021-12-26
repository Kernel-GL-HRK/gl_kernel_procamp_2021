KERNELDIR ?= ../output/build/linux-5.10.10/ #WARNING relative path

obj-m := ledirq_mod.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
