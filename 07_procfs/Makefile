obj-m += procfs_module.o
CFLAGS_procfs_module.o := -DDEBUG

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	dmesg -c
	insmod procfs_module.ko
	echo "123456" > /sys/kernel/basic_struct/list
	cat /sys/kernel/basic_struct/list
	echo "1234567890263321564654654654679874964321356" > /sys/kernel/basic_struct/list
	echo "Hello world!" > /sys/kernel/basic_struct/list
	cat /sys/kernel/basic_struct/list
	cat /proc/task07_module_entries/module_author
	cat /proc/task07_module_entries/write_requests
	cat /proc/task07_module_entries/read_requests
	rmmod procfs_module.ko
	dmesg
