KERNELDIR ?= ~/Development/Embedded/exercise8/buildroot_qemu_x64_image/build/linux-5.10.7/
obj-m += sum_module.o
all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod sum_module.ko first_param=10 second_param=200
	sudo rmmod sum_module.ko
	dmesg
