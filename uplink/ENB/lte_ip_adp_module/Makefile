TARGET = lte_ipadp_module
OBJS =  fsmcore.o ipadpfsm.o ipadp_module.o

EXTRA_CFLAGS += -w -g
CURRENT = $(shell uname -r)
KDIR = /home/redhat/Desktop/amc718_bsp_source_release3/kernel/linux-2.6.34.6
PWD = $(shell pwd)

obj-m      := $(TARGET).o

$(TARGET)-objs := $(OBJS)

default:
	make -C $(KDIR) M=$(PWD) modules ARCH=powerpc CROSS_COMPILE=/opt/freescale/usr/local/gcc-4.5.55-eglibc-2.11.55/powerpc-linux-gnu/bin/powerpc-linux-gnu-
clean:
	-rm -f *.o *.ko .*.cmd .*.flags *.mod.c
