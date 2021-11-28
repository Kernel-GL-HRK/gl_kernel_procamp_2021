KERNELDIR ?= ~/Development/Embedded/exercise8/buildroot_qemu_x64_image/build/linux-5.10.7/
CFLAGS_basic_struct_module.o := -DDEBUG
obj-m += basic_struct_module.o

all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
test:
	sh test_mod.sh