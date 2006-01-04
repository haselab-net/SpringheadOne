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

CFLAGS        = -I$(INSTALL_DIR)/include -I$(INSTALL_DIR)/libstdc++/include/c++/3.2.1 \
				-I$(INSTALL_DIR)/libstdc++/include/c++/3.2.1/sh-elf-ecos \
				-I$(SPRINGHEAD)/src
CXXFLAGS      = $(CFLAGS)
LDFLAGS       = -L$(INSTALL_DIR)/lib -T$(INSTALL_DIR)/lib/target.ld

OBJS          = Quaternion.o

# RULES

.PHONY: all clean

all:	a.mot

a.mot:	a.out
	sh-elf-objcopy -O srec a.out a.mot
	HDHCHost a.mot 0
	sh-elf-objdump -D a.out > a.txt

clean:
	-rm a.out $(OBJS) a.mot a.txt

a.out: $(OBJS) Makefile
	$(XLD) $(LDFLAGS) $(ECOS_GLOBAL_LDFLAGS) -o $@ $(OBJS) $(INSTALL_DIR)/libstdc++/lib/libstdc++.a

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
