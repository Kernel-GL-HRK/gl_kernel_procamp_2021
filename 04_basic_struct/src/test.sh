#/bin/bash
#make clean
#make
PATH=/sys/class/example-class

sudo dmesg -C
#make install
insmod procfs_rw.ko

echo "12345 67890" > /proc/example/buffer

cat /proc/example/buffer

sleep 1
cat /proc/example/buffer

sleep 5
cat /proc/example/buffer

#make remove
rmmod procfs_rw.ko

sudo dmesg



