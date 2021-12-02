#
# procfs example
#

TARGET = sysfs_rw

BUILD_KERNEL=/lib/modules/$(shell uname -r)/build
#BUILD_KERNEL := ~/projects/LinuxKernelCourse2.0Participants-summer2021/kernel/linux-stable/my01

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := main.o

else

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
	- $(shell echo "12345 67890" > /sys/class/example-class/list)
	- echo `cat /sys/class/example-class/list`

endif


