#include <stdio.h>

#define USE_QUADS

#include <SOLID/solid.h>

#include <3D/Point.h>
#include <3D/Quaternion.h>

#define SPACE_SIZE 20
#define NUM_ITER 10000

typedef struct MyObject {
  int id;
} MyObject; 

/* ARGSUSED */
void collide1(void * client_data, DtObjectRef obj1, DtObjectRef obj2,
	      const DtCollData *coll_data) {
}

/* ARGSUSED */
void collide2(void * client_data, DtObjectRef obj1, DtObjectRef obj2,
	      const DtCollData *coll_data) {
  FILE *stream = (FILE *)client_data;
  fprintf(stream, "Object %d interferes with object %d\n", 
	  (*(MyObject *)obj1).id, (*(MyObject *)obj2).id);
}

#ifdef STATISTICS
extern int num_box_tests;
#endif

main() {
  MyObject object1, object2;

  object1.id = 1;
  object2.id = 2;
  
  DtShapeRef shape = dtNewComplexShape();
  
  double a = 10; 
  double b = 2; 

  fprintf(stderr, "Loading a torus with a major radius of %d and a minor radius of %d%,\n", (int)a, (int)b); 
  
  const int n1 = 50;
  const int n2 = 50;

#ifdef USE_QUADS
  fprintf(stderr, "composed of %d quads...",n1 * n2); fflush(stderr); 
#else
  fprintf(stderr, "composed of %d triangles...", 2 * n1 * n2); fflush(stderr); 
#endif
  for (int uc = 0; uc < n1; uc++)
    for (int vc = 0; vc < n2; vc++) {
      double u1 = (2.0*M_PI*uc) / n1; 
      double u2 = (2.0*M_PI*(uc+1)) / n1; 
      double v1 = (2.0*M_PI*vc) / n2; 
      double v2 = (2.0*M_PI*(vc+1)) / n2; 
      
      double p1[3], p2[3], p3[3], p4[3];
      
      p1[0] = (a - b * cos(v1)) * cos(u1);
      p2[0] = (a - b * cos(v1)) * cos(u2);
      p3[0] = (a - b * cos(v2)) * cos(u1);
      p4[0] = (a - b * cos(v2)) * cos(u2);
      p1[1] = (a - b * cos(v1)) * sin(u1);
      p2[1] = (a - b * cos(v1)) * sin(u2);
      p3[1] = (a - b * cos(v2)) * sin(u1);
      p4[1] = (a - b * cos(v2)) * sin(u2);
      p1[2] = b * sin(v1);
      p2[2] = b * sin(v1);
      p3[2] = b * sin(v2);
      p4[2] = b * sin(v2);
      
#ifdef USE_QUADS
      dtBegin(DT_POLYGON);
      dtVertex(p1[0], p1[1], p1[2]);
      dtVertex(p2[0], p2[1], p2[2]);
      dtVertex(p4[0], p4[1], p4[2]);
      dtVertex(p3[0], p3[1], p3[2]);
      dtEnd();    
#else      
      dtBegin(DT_SIMPLEX);
      dtVertex(p1[0], p1[1], p1[2]);
      dtVertex(p2[0], p2[1], p2[2]);
      dtVertex(p3[0], p3[1], p3[2]);
      dtEnd();
      dtBegin(DT_SIMPLEX);
      dtVertex(p4[0], p4[1], p4[2]);
      dtVertex(p1[0], p1[1], p1[2]);
      dtVertex(p2[0], p2[1], p2[2]);
      dtEnd();
#endif  
    }
  fprintf(stderr, "done.\n");
  fprintf(stderr, "Building hierarchy..."); fflush(stderr);
  dtEndComplexShape();
  fprintf(stderr, "done.\n");
 
  dtCreateObject(&object1, shape); 
  dtCreateObject(&object2, shape); 

  dtDisableCaching();

  dtSetDefaultResponse(collide1, DT_SIMPLE_RESPONSE, stdout);

  int col_count = 0;
  Quaternion q;

  printf("Running %d tests at random placements\n", NUM_ITER);
  printf("in a space of size %d...\n", SPACE_SIZE);  
  for (int i = 0; i != NUM_ITER; ++i) {
    dtSelectObject(&object1);
    dtLoadIdentity();
    dtTranslate(rnd() * SPACE_SIZE, rnd() * SPACE_SIZE, rnd() * SPACE_SIZE);
    q = Quaternion::random();
    dtRotate(q[X], q[Y], q[Z], q[W]);

    dtSelectObject(&object2);
    dtLoadIdentity();
    dtTranslate(rnd() * SPACE_SIZE, rnd() * SPACE_SIZE, rnd() * SPACE_SIZE);
    q = Quaternion::random();
    dtRotate(q[X], q[Y], q[Z], q[W]);

    if (dtTest()) ++col_count;
  }
  printf("done\n");

  cout << "Number of collisions: " << col_count << endl;
#ifdef STATISTICS
  cout << "Number of sep_axis calls: " << num_box_tests << endl;
#endif
  dtDeleteObject(&object1);
  dtDeleteObject(&object2);
  dtDeleteShape(shape);

  return 0;
}


