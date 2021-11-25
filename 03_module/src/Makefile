CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
KDIR_SRC = /usr/src/$(CURRENT)/

TARGET = test_module

obj-m      := $(TARGET).o

all: default clean

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *.symvers *~ *.*~ TODO.*
	@rm -fR .tmp*
	@rm -rf .tmp_versions

disclean: clean
	@rm -f *.ko

test:
	#sudo dmesg -C
	sudo insmod $(TARGET).ko val1=5 val2=3
	sudo rmmod $(TARGET)

remove:
	sudo rmmod $(TARGET)
	sudo dmesg

check:
	$KDIR_SRC/scripts/checkpath.py .
