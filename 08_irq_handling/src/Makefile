TARGET = led_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := led_mod.o

else


ifneq ($(QUEMU),)
BUILD_KERNEL=~/projects/LinuxKernelCoursePro/kernel/buildroot-2021.02.7/output/build/linux-5.10.10/
$(info  1 $(QUEMU))
else
BUILD_KERNEL=/lib/modules/$(shell uname -r)/build
$(info  2 $(QUEMU))
endif


KERNELDIR := $(BUILD_KERNEL)

.PHONY: all clean

all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
install: 
	sudo insmod ./$(TARGET).ko
	#- lsmod | grep head -n
remove:
	sudo rmmod $(TARGET)
	#- lsmod | grep head -n
test:
	#sudo dmesg -C
	sudo insmod $(TARGET).ko
	sudo rmmod $(TARGET)

check:
	- $(BUILD_KERNEL)/scripts/checkpatch.pl -f --no-tree --fix ./main.c

endif


