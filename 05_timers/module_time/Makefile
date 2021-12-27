KERNEL_SRC := ../../../qemu/kernel

obj-m := module_time.o

all:
	@make -C $(KERNEL_SRC) M=$(CURDIR) -j`nproc`

send:
	scp *.ko lavr:

clean:
	@rm -f *.o .*.cmd *.ko *.mod.c *.mod Module.symvers modules.order
