ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := potent_array.o

else
# normal makefile
# path to kernel dir
KDIR := ../../../../linux-socfpga
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules
	
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif
