KERNELDIR ?= ../../output/build/linux-5.15/ #WARNING relative path

obj-m := sum_sub.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
