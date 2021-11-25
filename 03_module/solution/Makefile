obj-m += sum_module.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod sum_module.ko first_param=10 second_param=200
	sudo rmmod sum_module.ko
	dmesg
