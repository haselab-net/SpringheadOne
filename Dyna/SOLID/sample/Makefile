include ../Make-config

INCDIR = ../include
LIBDIR = ../lib

CPPFLAGS = -I$(INCDIR) 
LDFLAGS = -L$(LIBDIR)  
LDLIBS = -lsolid $(QHULL_LD) -lm

default: sample x_wing teapot

clean:
	-rm *.o gmon.out core

realclean: clean
	-rm sample x_wing teapot


