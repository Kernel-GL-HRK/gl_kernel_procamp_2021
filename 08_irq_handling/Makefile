ORANGEKERN ?= ~/Orange/buildroot-2021.11.1/output/build/linux-5.12.2/ #relative path
LOCALKERN ?= /lib/modules/$(shell uname -r)/build

obj-m := irq_module.o

all: clean build

build:
	$(MAKE) -C $(ORANGEKERN) M=$(PWD) modules

clean:
	$(MAKE) -C $(ORANGEKERN) M=$(PWD) clean
