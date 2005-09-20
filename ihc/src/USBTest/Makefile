# Mostly written by Jonathan Larmour, Red Hat, Inc.
# Reference to ecos.mak added by John Dallaway, eCosCentric Limited, 2003-01-20
# This file is in the public domain and may be used for any purpose

# Usage:   make INSTALL_DIR=/path/to/ecos/install

#INSTALL_DIR=$$(INSTALL_DIR) # override on make command line
INSTALL_DIR=/home/hase/C/Project/HDHC/OS/SH4CF_install

include $(INSTALL_DIR)/include/pkgconf/ecos.mak

XCC           = $(ECOS_COMMAND_PREFIX)gcc
XCXX          = $(XCC)
XLD           = $(XCC)

CFLAGS        = -O3 -I$(INSTALL_DIR)/include
#CFLAGS        += -save-temps
CXXFLAGS      = $(CFLAGS)
LDFLAGS       = -L$(INSTALL_DIR)/lib -T$(INSTALL_DIR)/lib/target.ld

OBJS          = main.o hwsetup.o TMUCtrl.o OutPort.o setting.o         \
                SpidarThread.o PhysicsThread.o USBThread.o USB_INTR.o  \
                qp_prog01.o                                            \
                ihcdevicemanager.o ihcdevicemanagerbase.o IHCDebuger.o \
                ihcspidar4quad.o ihcmotor.o \
                SH4Data.o SH4DataSet.o SH4Matrix.o SH4Vector.o

# RULES

.PHONY: all clean

all:	a.mot

a.mot:	a.out
	sh-elf-objcopy -O srec --srec-len 70 a.out a.mot
	cp a.mot ../../host/HDSpidarSample01/IHCSpidarAmp03.mot
	sh-elf-objdump -D a.out > a.txt

clean:
	-rm a.out $(OBJS)

a.out: $(OBJS)
	$(XLD) $(LDFLAGS) $(ECOS_GLOBAL_LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(XCC) -c -o $*.o $(CFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.cxx
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.C
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.cc
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

%.o: %.cpp
	$(XCXX) -c -o $*.o $(CXXFLAGS) $(ECOS_GLOBAL_CFLAGS) $<

