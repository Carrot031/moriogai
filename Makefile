.PHONY: all clean

KERNELSRCDIR = /usr/lib/modules/$(shell uname -r)/build
BUILD_DIR := $(shell pwd)
VERBOSE=0

obj-m:=moriogai.o

all:
	make -C $(KERNELSRCDIR) M=$(BUILD_DIR) KBUILD_VERBOSE=$(VERBOSE) modules

clean:
	rm -f *.o *.ko *.mod.c *~
