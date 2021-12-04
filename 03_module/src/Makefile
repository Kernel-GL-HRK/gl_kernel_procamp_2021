KERNELDIR ?=../output/build/linux-5.10.7/ # WARNING: relative path

obj-m := example.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
