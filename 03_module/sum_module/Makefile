obj-m := sum_module.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: all
	sudo dmesg -C
	sudo insmod sum_module.ko first_op=5 second_op=2
	sudo rmmod sum_module
	sudo dmesg | grep sum_module
