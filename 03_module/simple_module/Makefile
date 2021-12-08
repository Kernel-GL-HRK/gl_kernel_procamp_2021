KERNEL_SRC := ../../../kernel

obj-m := smodule.o

all:
	@make -C $(KERNEL_SRC) M=$(CURDIR) -j`nproc`

send:
	scp *.ko lavr:/home/lavr

clean:
	@rm -f *.o .*.cmd *.ko *.mod.c *.mod Module.symvers modules.order
