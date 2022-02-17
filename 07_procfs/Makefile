KERNELDIR ?= ~/Buildroot/buildroot-2021.11.1/output/build/linux-5.15/

obj-m := procfs_module.o

all: clean build

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
