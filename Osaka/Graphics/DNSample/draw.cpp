// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "DispNode.h"

using namespace VRBase;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void RotateView(void);


int		scWidth, scHeight;

//int		dlField;
//int		dlPenguin;
//Affinef posPenguin;
vector<UTRef<DispNode> > dnTree;

////////////////////////////////////////////////////////////////////////////////
//	GLUT callback functions

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(180/M_PI * vieworient[3], vieworient[0], vieworient[1], vieworient[2]);
	glTranslatef(-viewpos[0], -viewpos[1], -viewpos[2]);
	RotateView();

//	glCallList(dlField);
//	glPushMatrix();
//	glMultMatrixf(posPenguin);
//	glCallList(dlPenguin);
//	glPopMatrix();

	DrawDLs( dnTree );

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(180/M_PI * fieldOfView, (GLfloat)width / height, .1, 10000);
	glMatrixMode(GL_MODELVIEW);

	scWidth = width; scHeight = height;
}

