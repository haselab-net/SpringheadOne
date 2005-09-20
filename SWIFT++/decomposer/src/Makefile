.SUFFIXES: .o .cpp

# Header files
HDS = -I../include -I$(QHULL_DIR) -I../RAPID -I../../include

# Library directories
LIBDIRS_C = -L$(QHULL_DIR) -L../../lib -L../RAPID 

LIBDIRS_G = -L/usr/X11R6/lib $(LIBDIRS_C)

# Libraries
OTHER_LIBS = -lm #-ldl

LIBS_C = -lRAPIDdecomp -lSWIFT++decomp -lqhull $(OTHER_LIBS)

LIBS_G = $(LIBS_C) -lGLU -lGL -ltcl8.0 -ltk8.0 -lXext -lXi -lXmu -lX11 \
         $(OTHER_LIBS)

# Object files
OBJECT_C = convex.o cvxutils.o gui.o io.o main.o

OBJECT_G = $(OBJECT_C) guidraw.o togl.o viewer.o

SOURCES = $(OBJECT_G:.o=.cpp)

.cpp.o:
	$(CC) -c $(CFLAGS) -DSWIFT_DECOMP $(HDS) $*.cpp

decomposer_c: clean depend $(OBJECT_C)
	$(CC) $(CFLAGS) -o decomposer $(OBJECT_C) $(LIBDIRS_C) $(LIBS_C)

decomposer_g: clean depend $(OBJECT_G)
	$(CC) $(CFLAGS) -o decomposer $(OBJECT_G) $(LIBDIRS_G) $(LIBS_G)

depend:
	makedepend -Y -- $(CFLAGS) $(HDS) -- $(SOURCES)

clean:
	rm -f *.o

# DO NOT DELETE

