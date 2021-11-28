KDIR ?= ~/work/buildroot-2021.02.7/output/build/linux-5.10.7
CHECKPATCH := $(KDIR)/scripts/checkpatch.pl

SRC := argsmod.c
OBJS := $(SRC:.c=.o)

obj-m += $(OBJS)

all:
	$(CHECKPATCH) -f $(SRC) || exit 1
	$(MAKE) -C $(KDIR) M=$(PWD) modules

default: all

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

