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