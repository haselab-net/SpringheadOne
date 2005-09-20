LIBNAME = solid

SRCS = 	Transform.cpp Convex.cpp Box.cpp Cone.cpp Cylinder.cpp Sphere.cpp \
	Simplex.cpp Polygon.cpp Polyhedron.cpp Complex.cpp \
	BBoxTree.cpp Endpoint.cpp Object.cpp Response.cpp RespTable.cpp \
	C-api.cpp
OBJS = $(SRCS:.cpp=.o)

include ../Make-config

INCDIR = ../include
LIBDIR = ../lib

CPPFLAGS = -I$(INCDIR) $(QHULL_CPP)
LDFLAGS = -L$(LIBDIR) 
LDLIBS = -l$(LIBNAME) $(QHULL_LD) -lm

LIB.a = $(LIBDIR)/lib$(LIBNAME).a
LIB.so = $(LIBDIR)/lib$(LIBNAME).so

default: $(LIB.a)

$(LIB.a): $(OBJS)
	ar ruv $@ $?

$(LIB.so): $(OBJS)
	ld -n32 -shared -all -o $@ $?

clean:
	-rm *.o 

realclean: clean
	-rm $(LIBDIR)/*.a $(LIBDIR)/*.so 


