KERNELDIR ?= ../../output/build/linux-5.12.2/ #WARNING relative path

TARGET = procfs_test

obj-m := $(TARGET).o
CFLAGS_$(TARGET).o := -std=gnu11 -DDEBUG -Wno-declaration-after-statement

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

