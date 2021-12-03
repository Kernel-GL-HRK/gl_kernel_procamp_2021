KERNELDIR ?= ../output/build/linux-5.10.7/ #WARNING relative path

obj-m := hello_mod.o

all: clean build

build: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
