KERNEL_DIR ?= ../buildroot/output/build/linux-5.15

obj-m += kernel_time.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
