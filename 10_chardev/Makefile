KERNELDIR ?= ../../output/build/linux-5.10.10/ #WARNING relative path

TARGET = chrdev_mod

obj-m := $(TARGET).o
CFLAGS_$(TARGET).o := -std=gnu11 -DDEBUG -Wno-declaration-after-statement

all: clean build ioctrl

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf ioctrl_usrspace

ioctrl:
	$(CROSS_COMPILE)gcc ioctrl_usrspace.c -o ioctrl_usrspace
