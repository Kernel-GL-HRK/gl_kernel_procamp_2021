KERNELDIR ?= ../../../buildroot/output/build/linux-5.15/ #WARNING relative path

obj-m := timediff.o
CFLAGS_timediff.o := -DDEBUG

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

