include $(IHC_ROOT)/src/defs.mak

OBJS = main.o
LIBS = $(IHC_ROOT)/lib/libSpr.a $(INSTALL_DIR)/libstdc++/lib/libstdc++.a

include $(IHC_ROOT)/src/rammot.mak
