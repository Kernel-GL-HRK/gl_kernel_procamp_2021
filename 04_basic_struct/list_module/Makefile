EXTRA_CFLAGS += -O3 -std=gnu89 --no-warnings

OBJS = list_module.o linked_list.o

obj-m := lmod.o
lmod-objs := $(OBJS)

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

lmod.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test1: all
	sudo dmesg -C
	sudo insmod lmod.ko
	echo "Test string" | sudo tee /sys/kernel/list/list
	cat /sys/kernel/list/list
	sudo rmmod lmod
	sudo dmesg | grep lmod

test2: all
	sudo dmesg -C
	sudo insmod lmod.ko
	echo "Test string 1" | sudo tee /sys/kernel/list/list
	echo "Test string 2" | sudo tee /sys/kernel/list/list
	echo "Test string 3" | sudo tee /sys/kernel/list/list
	cat /sys/kernel/list/list
	sudo rmmod lmod
	sudo dmesg | grep lmod