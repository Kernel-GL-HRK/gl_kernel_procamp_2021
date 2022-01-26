
KERNELDIR ?=/home/valenti/Development/Embedded/exercise8/build_orange_zero_image/build/linux-5.10.10/

ifneq ($(KERNELRELEASE),)
	obj-m := procfs_module.o
else

all:
	$(MAKE) CFLAGS_MODULE="-DDEBUG " -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
endif
