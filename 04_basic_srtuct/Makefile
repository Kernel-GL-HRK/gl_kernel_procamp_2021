#KERNELDIR ?= ~/Development/Embedded/exercise8/buildroot_qemu_x64_image/build/linux-5.10.7/
KERNELDIR ?= ~/soft/buildroot-2021.11-rc2/output/build/linux-5.15/
CFLAGS_basic_struct.o := -DDEBUG
obj-m += basic_struct.o

all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
test:
	sh test_S_mod.sh