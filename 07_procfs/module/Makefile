KERNELDIR ?= ../../../buildroot-2021.02.7/BUILD/build/linux-5.10.7  # WARNING relative path
#KERNELDIR ?= ../../../buildroot-2021.02.7/orangepi_zero/build/linux-5.10.10

obj-m := my_module.o

all:
	$(MAKE) -C ../../../buildroot-2021.02.7/orangepi_zero/build/linux-5.10.10 M=$(PWD) modules ARCH=arm CROSS_COMPILE=../../host/bin/arm-buildroot-linux-uclibcgnueabihf-

for_qemu:
	$(MAKE) -C ../../../buildroot-2021.02.7/BUILD/build/linux-5.10.7 M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf tags
