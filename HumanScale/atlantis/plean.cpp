//plean.c
#include <math.h>
#include <GL/glut.h>
#include "atlantis.h"

void
drawplean(){

  glPushMatrix();
  glTranslatef(-1000000,-100000,-10000000);
  //glTranslatef(-3000000, -4000000, -10000000);
  glColor4f(1.0, 1.0, 1.0, 1.0);

  glBegin(GL_QUADS);

  glVertex3f(0.0, 0.0, 0.0);
  glTexCoord2f(0.0, 0.0);  
  glVertex3f(0.0, 0.0, 100000000.0);
  glTexCoord2f(0, 1000);
  glVertex3f(100000000.0, 0.0, 100000000.0);
  glTexCoord2f(1000, 1000);
  glVertex3f(100000000.0,0.0, 0.0);
  glTexCoord2f(1000.0, 0.0);
  glEnd();   


  glPopMatrix();
} 
