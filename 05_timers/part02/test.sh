#/bin/bash
lsmod | grep test_module
if [ $? == 0 ]; then
    make remove
fi

sudo dmesg -C
sleep 1
echo -e "start test" | sudo tee /dev/msg
make clean
make
make test
#sudo insmod ./test_module.ko 
#sudo rmmod test_module  
#cat /sys/class/example-class/xxx  
echo -e "start test" | sudo tee /dev/msg
echo -e "" | sudo tee /dev/msg
sleep 1
dmesg
