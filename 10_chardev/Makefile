KERNELDIR ?= ../output/build/linux-5.10.10/ #WARNING relative path

obj-m := ioctl_mod.o

all: clean build ioctrl

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf server

ioctrl:
	$(CROSS_COMPILE)gcc server.c -o server
	$(CROSS_COMPILE)gcc ioctl_app.c -o ioctl_app	