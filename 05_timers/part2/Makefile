BUILD_KERNEL=/home/dtretiakov/Desktop/Linux-Kernel-ProCamp-2021/linux-stable


obj-m+=time-module.o
all: build rm-trash file-push-to-qemu
	
build:
	make -C $(BUILD_KERNEL) M=$(PWD) modules
	
clean:
	make -C $(BUILD_KERNEL) M=$(PWD) clean

rm-trash:
	rm -f  *.mod *.order *.o *. *.symvers *.mod.c .*.cmd

file-push-to-qemu:
	sshpass -p 'pass' scp -P 8022 *.ko script-test user@localhost:~



# qemu
# !6770
