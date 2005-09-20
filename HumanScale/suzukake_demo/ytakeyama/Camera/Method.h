#ifndef __METHOD_H__
#define __METHOD_H__
//#include "camera.h"
#include <GL/glut.h>
//#include <iostream.h>
//#include <iomanip.h>
#define MAXPOINTS 100
#define EDGE_MAX 12


void init(void);
void resize(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
//void motion(int x, int y);
void cube(void);
void idle(void);

int* ReturnAngle();


#endif

