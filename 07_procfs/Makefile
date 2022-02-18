KERNELDIR ?= ~/Buildroot/buildroot-2021.11.1/output/build/linux-5.15/

obj-m := procfs_module.o

all: clean build

build:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: all
	sudo insmod procfs_module.ko
	echo "Author's name entry output" > log.txt
	cat /proc/my_folder/author >> log.txt
	echo "Test1" | sudo tee /proc/my_folder/write
	echo "Test2" | sudo tee /proc/my_folder/read
	echo '\nWrite calls counter entry output' >> log.txt
	cat /proc/my_folder/write >> log.txt
	echo '\nRead calls counter entry output' >> log.txt
	cat /proc/my_folder/read >> log.txt
	echo '\n' >> log.txt
	sudo rmmod procfs_module
	cat log.txt
