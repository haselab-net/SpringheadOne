INSTALL_DIR=$(IHC_ROOT)/ihc_install
include $(INSTALL_DIR)/include/pkgconf/ecos.mak
XCC           = $(ECOS_COMMAND_PREFIX)gcc
XCXX          = $(XCC)
XLD           = $(XCC)
XAR           = $(ECOS_COMMAND_PREFIX)ar

CFLAGS        = -I$(IHC_ROOT)/src \
				-I$(SPRINGHEAD)/src \
				-I$(INSTALL_DIR)/libstdc++/include/c++/3.2.1 \
				-I$(INSTALL_DIR)/libstdc++/include/c++/3.2.1/sh-elf-ecos \
				-I$(INSTALL_DIR)/include 

CXXFLAGS      = $(CFLAGS)
LDFLAGS       = -L$(IHC_ROOT)/lib -L$(INSTALL_DIR)/lib -L$(INSTALL_DIR)/libstdc++/lib \
				-T$(INSTALL_DIR)/lib/target.ld
