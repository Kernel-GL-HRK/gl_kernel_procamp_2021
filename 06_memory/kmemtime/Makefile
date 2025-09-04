KERNELDIR ?= ../../../buildroot/output/build/linux-5.15/ #WARNING relative path

obj-m := kmemtime.o
CFLAGS_kmemtime.o := -DDEBUG -std=gnu11 -Wno-declaration-after-statement

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

