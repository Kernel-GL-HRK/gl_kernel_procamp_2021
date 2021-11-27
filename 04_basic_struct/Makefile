KERNELDIR ?= ~/Development/Embedded/exercise8/buildroot_qemu_x64_image/build/linux-5.10.7/
obj-m += basic_struct_module.o


all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod basic_struct_module.ko
	echo 1234 > /sys/basic_struct/list
	echo 567890 > /sys/basic_struct/list
	echo "Hello world" > /sys/basic_struct/list
	cat /sys/basic_struct/list
	sudo rmmod basic_struct_module.ko
	dmesg
