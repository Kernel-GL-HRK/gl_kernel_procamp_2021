obj-m += time_mod_driver.o
CFLAGS_time_mod_driver.o := -DDEBUG

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	- sudo insmod time_mod_driver.ko
	- sudo cat /sys/kernel/timer_module_dir/rw_node
	- sleep 0.4
	- sudo cat /sys/kernel/timer_module_dir/rw_node
	- sudo cat /sys/kernel/timer_module_dir/absolute_time_node
	- sleep 0.3
	- sudo cat /sys/kernel/timer_module_dir/rw_node
	- sudo cat /sys/kernel/timer_module_dir/rw_node
	- sudo cat /sys/kernel/timer_module_dir/rw_node
	- sudo rmmod time_mod_driver.ko
	dmesg
