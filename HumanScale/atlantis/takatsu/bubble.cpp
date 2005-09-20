//bubble.c
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "atlantis.h"
void drawbubbles(void){
  int i;
  for (i = 0; i<NUM_BUBBLES ; i++){
    bub[i].x += bub[i].r * cos (bub[i].theta/RAD);
    bub[i].y += bub[i].r *  sin (bub[i].theta/RAD);
    bub[i].z += bub[i].v;
    /*if(((rand()%NUM_BUBBLES) == 0) && (bub[i].z > 20000)){bub[i].z = rand() % 6000 -20000;}*/
    //if(bub[i].z > 30000){bub[i].z = rand() % 6000 -20000;}
    if(bub[i].z > 30000){bub[i].z = 30000 % 6000 -20000;}
    glPushMatrix();
    glTranslatef(bub[i].y, bub[i].z, -bub[i].x);
    //glColor4f(bub[i].color, bub[i].color, 1.0, 0.1);
    glColor4f(1.0,1.0, 1.0, 0.1);
    glutSolidSphere(bub[i].size,10,10);
    glPopMatrix();
  }
}

