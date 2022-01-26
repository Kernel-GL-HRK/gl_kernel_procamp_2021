KERNELDIR ?= ../../output/build/linux-5.10.10/ #WARNING relative path

TARGET = led_mod_irq

obj-m := $(TARGET).o
CFLAGS_$(TARGET).o := -std=gnu11 -DDEBUG -Wno-declaration-after-statement

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
