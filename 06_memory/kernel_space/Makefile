KERNEL_DIR ?= ../buildroot/output/build/linux-5.15

obj-m += module_memory.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
