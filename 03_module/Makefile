obj-m += mymodule.o

# Build for current kernel

#all:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

#clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


# Build for another kernel

all:
	make -C /home/user/Files_Slava/GL_dev/C/linux-kernel/buildroot-2021.02.9/output/build/linux-5.10.7/ M=$(PWD) modules
	
clean:
	make -C /home/user/Files_Slava/GL_dev/C/linux-kernel/buildroot-2021.02.9/output/build/linux-5.10.7/ M=$(PWD) clean
