KERNELDIR ?= ../output/build/linux-5.10.7/ #WARNING relative path

obj-m := memory_tst_mod.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
