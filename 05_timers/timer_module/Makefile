REL_TIME=0
ABS_TIME=1

obj-m := timer_module.o

all: 
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: all
	sudo dmesg -C
	sudo insmod timer_module.ko
	echo $(ABS_TIME) | sudo tee /sys/kernel/time/time
	cat /sys/kernel/time/time
	echo $(REL_TIME) | sudo tee /sys/kernel/time/time
	cat /sys/kernel/time/time
	sleep 0.25
	cat /sys/kernel/time/time
	sleep 0.5
	cat /sys/kernel/time/time
	echo $(ABS_TIME) | sudo tee /sys/kernel/time/time
	cat /sys/kernel/time/time
	sudo rmmod timer_module
	sudo dmesg | grep timer_module
