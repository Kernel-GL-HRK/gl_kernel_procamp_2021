KERNELDIR ?= ../../buildroot/output/build/linux-5.15/ #WARNING relative path

obj-m := string_storage.o
CFLAGS_string_storage.o := -DDEBUG -Wno-declaration-after-statement

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

