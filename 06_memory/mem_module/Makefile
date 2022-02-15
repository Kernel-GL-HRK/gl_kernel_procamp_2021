KMALLOC=1
KZALLOC=2
VMALLOC=3
GET_FREE_PAGES=4

obj-m := mem_module.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: all
	sudo dmesg -C
	echo 'KMALLOC LOG\n' > log.txt
	sudo insmod mem_module.ko arg=$(KMALLOC)
	sudo rmmod mem_module
	sudo dmesg | grep mem_module >> log.txt
	sudo dmesg -C
	echo '\nKZALLOC LOG\n' >> log.txt
	sudo insmod mem_module.ko arg=$(KZALLOC)
	sudo rmmod mem_module
	sudo dmesg | grep mem_module >> log.txt
	sudo dmesg -C
	echo '\nVMALLOC LOG\n' >> log.txt
	sudo insmod mem_module.ko arg=$(VMALLOC)
	sudo rmmod mem_module
	sudo dmesg | grep mem_module >> log.txt
	sudo dmesg -C
	echo '\nGET_FREE_PAGES LOG\n' >> log.txt
	sudo insmod mem_module.ko arg=$(GET_FREE_PAGES)
	sudo rmmod mem_module
	sudo dmesg | grep mem_module >> log.txt
	sudo dmesg -C
	cat log.txt
