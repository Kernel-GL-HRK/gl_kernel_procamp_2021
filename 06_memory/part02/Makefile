TARGET=test_mem
BIN=../bin

obj-m = $(TARGET).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: #all
	sudo dmesg -C
	- sudo insmod ./$(TARGET).ko
	#sudo rmmod $(TARGET)
	sudo dmesg

