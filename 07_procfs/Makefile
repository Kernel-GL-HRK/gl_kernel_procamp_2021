KERNELDIR ?= ../output/build/linux-5.10.10

obj-m := wimbledon.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
