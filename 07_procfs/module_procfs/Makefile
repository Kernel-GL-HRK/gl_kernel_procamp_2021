KERNEL_SRC := ../../../orangepi/kernel

export ARCH := arm
export CROSS_COMPILE := arm-linux-
export PATH := "/w/orangepi/image/host/bin:$(PATH)"

obj-m := module_procfs.o

all:
	@PATH=$(PATH) make -C $(KERNEL_SRC) M=$(CURDIR) -j`nproc`

send:
	scp *.ko olavr:

clean:
	@rm -f *.o .*.cmd *.ko *.mod.c *.mod Module.symvers modules.order
