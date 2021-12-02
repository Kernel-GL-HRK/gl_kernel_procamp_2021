	dmesg -c
	insmod basic_struct_module.ko
	echo "123456" > /sys/kernel/basic_struct/list
	cat /sys/kernel/basic_struct/list
	echo "1234567890263321564654654654679874964321356" > /sys/kernel/basic_struct/list
	echo "Hello world!" > /sys/kernel/basic_struct/list
	cat /sys/kernel/basic_struct/list
	rmmod basic_struct_module.ko
	dmesg
