KERNELDIR ?= ../../../buildroot-2021.02.7/BUILD/build/linux-5.10.7  # WARNING relative path

obj-m := my_module.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf tags
