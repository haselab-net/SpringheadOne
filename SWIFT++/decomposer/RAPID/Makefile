.SUFFIXES: .o .cpp

# Object files
OBJECT = RAPID.o build.o collide.o overlap.o

SOURCES = $(OBJECT:.o=.cpp)

.cpp.o:
	$(CC) -c $(CFLAGS) $*.cpp

libRAPIDdecomp.a: clean $(OBJECT)
	ar ruvs libRAPIDdecomp.a $(OBJECT)
	rm -f *.o

depend:
	makedepend -Y -- -- $(SOURCES)

clean:
	rm -f *.o *.a

# DO NOT DELETE

RAPID.o: RAPID_version.h RAPID.h RAPID_private.h matvec.h overlap.h obb.h
build.o: RAPID_version.h RAPID.h RAPID_private.h matvec.h moments.h obb.h
collide.o: RAPID_version.h RAPID.h RAPID_private.h matvec.h overlap.h obb.h
overlap.o: RAPID_version.h matvec.h
