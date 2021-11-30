KERNELDIR ?= ../output/build/linux-5.10.7

obj-m := wimbledon.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
