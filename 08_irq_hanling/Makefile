
KERNEL_DIR ?= ~/orange/buildroot-2021.02.8/output/build/linux-5.10.10

obj-m += led_mod.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
