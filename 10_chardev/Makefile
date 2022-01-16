KERNEL_DIR ?= ~/orange/buildroot-2021.02.8/output/build/linux-5.10.10
CC=arm-buildroot-linux-uclibcgnueabihf-gcc

obj-m += chardev_mod.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean

app:
	$(CC) user_app.c -o user_app
