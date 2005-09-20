.SUFFIXES: .o .cpp

HDS = -I../include -I$(QHULL_DIR)

# Object files
OBJECT = fileio.o \
         lut.o \
         mesh.o \
         mesh_utils.o \
         object.o \
         pair.o \
         pqueue.o \
         scene.o

SOURCES = $(OBJECT:.o=.cpp)

.cpp.o:
	$(CC) -c $(CFLAGS) $(HDS) $*.cpp

all: libSWIFT++.a

libSWIFT++.a: clean depend $(OBJECT)
	ar ruvs ../lib/libSWIFT++.a $(OBJECT)
	rm *.o

libSWIFT++decomp.a: depend $(OBJECT)
	ar ruvs ../lib/libSWIFT++decomp.a $(OBJECT)
	rm *.o

depend:
	makedepend -Y -- $(HDS) -- $(SOURCES)

clean:
	rm -f *.o

# DO NOT DELETE

fileio.o: ../include/SWIFT.h ../include/SWIFT_config.h
fileio.o: ../include/SWIFT_common.h ../include/SWIFT_array.h
fileio.o: ../include/SWIFT_mesh.h ../include/SWIFT_linalg.h
fileio.o: ../include/SWIFT_lut.h ../include/SWIFT_fileio.h
lut.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
lut.o: ../include/SWIFT_linalg.h ../include/SWIFT_mesh.h
lut.o: ../include/SWIFT_array.h ../include/SWIFT_lut.h
mesh.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
mesh.o: ../include/SWIFT_linalg.h ../include/SWIFT_array.h
mesh.o: ../include/SWIFT_mesh.h ../include/SWIFT_lut.h
mesh.o: ../include/SWIFT_mesh_utils.h /usr/local/qhull/qhull_a.h
mesh.o: /usr/local/qhull/qhull.h /usr/local/qhull/user.h
mesh.o: /usr/local/qhull/mem.h /usr/local/qhull/qset.h
mesh.o: /usr/local/qhull/geom.h /usr/local/qhull/merge.h
mesh.o: /usr/local/qhull/poly.h /usr/local/qhull/io.h /usr/local/qhull/stat.h
mesh_utils.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
mesh_utils.o: ../include/SWIFT_linalg.h ../include/SWIFT_array.h
mesh_utils.o: ../include/SWIFT_mesh.h ../include/SWIFT_lut.h
mesh_utils.o: ../include/SWIFT_mesh_utils.h /usr/local/qhull/qhull_a.h
mesh_utils.o: /usr/local/qhull/qhull.h /usr/local/qhull/user.h
mesh_utils.o: /usr/local/qhull/mem.h /usr/local/qhull/qset.h
mesh_utils.o: /usr/local/qhull/geom.h /usr/local/qhull/merge.h
mesh_utils.o: /usr/local/qhull/poly.h /usr/local/qhull/io.h
mesh_utils.o: /usr/local/qhull/stat.h
object.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
object.o: ../include/SWIFT_linalg.h ../include/SWIFT_mesh.h
object.o: ../include/SWIFT_array.h ../include/SWIFT_lut.h
object.o: ../include/SWIFT_object.h ../include/SWIFT_pair.h
object.o: ../include/SWIFT_boxnode.h
pair.o: ../include/SWIFT_config.h ../include/SWIFT_common.h
pair.o: ../include/SWIFT_linalg.h ../include/SWIFT_array.h
pair.o: ../include/SWIFT_mesh.h ../include/SWIFT_lut.h
pair.o: ../include/SWIFT_object.h ../include/SWIFT_pair.h
pair.o: ../include/SWIFT_boxnode.h
scene.o: ../include/SWIFT.h ../include/SWIFT_config.h
scene.o: ../include/SWIFT_common.h ../include/SWIFT_array.h
scene.o: ../include/SWIFT_linalg.h ../include/SWIFT_mesh.h
scene.o: ../include/SWIFT_lut.h ../include/SWIFT_mesh_utils.h
scene.o: /usr/local/qhull/qhull_a.h /usr/local/qhull/qhull.h
scene.o: /usr/local/qhull/user.h /usr/local/qhull/mem.h
scene.o: /usr/local/qhull/qset.h /usr/local/qhull/geom.h
scene.o: /usr/local/qhull/merge.h /usr/local/qhull/poly.h
scene.o: /usr/local/qhull/io.h /usr/local/qhull/stat.h
scene.o: ../include/SWIFT_boxnode.h ../include/SWIFT_object.h
scene.o: ../include/SWIFT_pair.h ../include/SWIFT_fileio.h
