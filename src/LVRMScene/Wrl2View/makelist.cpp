// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#define GLUT_NO_LIB_PRAGMA
#include <GL/glut.h>

#include "type.h"

namespace Spr {

GLUquadricObj* qobj;

////////////////////////////////////////////////////////////////////////////////
//	init list tree

void InitList(TSts* sts)
{
	TSts::iterator	it;
	for(it = sts->begin(); it != sts->end(); ++it)
	  (*it)->DoAction(&TType::Init);	// init stuff is here
}

////////////////////////////////////////////////////////////////////////////////
//	draw display list

int DrawList(TSts* sts)
{
	TSts::iterator	it;

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, GL_TRUE);

	int	display_num = glGenLists(1);
	glNewList(display_num, GL_COMPILE);
	for(it = sts->begin(); it != sts->end(); ++it)
	  (*it)->DoAction(&TType::Draw);	// draw stuff is here
	glEndList();

	gluDeleteQuadric(qobj);

	return display_num;
}

}	//	namespace Spr
