KERNELDIR ?= ../../buildroot/buildroot-2021.02.7/output/build/linux-5.10.10/ #WARNING relative path

obj-m := kmodule_proc_fs.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
