BUILD_KERNEL=/home/dtretiakov/Desktop/Linux-Kernel-ProCamp-2021/buildroot/output/build/linux-5.12.2
export ARCH=arm
export CROSS_COMPILE=/home/dtretiakov/Desktop/Linux-Kernel-ProCamp-2021/buildroot/output/host/usr/bin/arm-linux-


obj-m+=kobject-module-update.o
all: build rm-trash file-push-to-qemu
	
build:
	make -C $(BUILD_KERNEL) M=$(PWD) modules
	
clean:
	make -C $(BUILD_KERNEL) M=$(PWD) clean

rm-trash:
	rm -f  *.mod *.order *.o *. *.symvers *.mod.c .*.cmd

file-push-to-qemu:
	sshpass -p 'pass' scp -P 22 *.ko root@192.168.3.91:~



# qemu
# !6770
