KERNELDIR ?= ~/soft/buildroot-2021.11-rc2/output/build/linux-5.15/
obj-m += modul_add.o
all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod modul_add znach_1=10 znach_2=200
	sudo rmmod modul_add.ko
	dmesg