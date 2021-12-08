KERNELDIR ?= ../../buildroot-2021.02.7/output/build/linux-5.10.7/ #WARNING relative path

obj-m:= values.o

all: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	