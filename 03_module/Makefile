ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m   := main.o
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
move:
	cd ../ && sudo cp -r 03_module /srv/nfs/busybox/tmp/
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif