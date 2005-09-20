// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "DispNode.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <crtdbg.h>
#include <iostream>

using namespace VRBase;

//extern int dlPenguin;
//extern int dlField;
Affinef *posPenguin;
vector<UTRef<DispNode> > dnPen2;
extern	vector<UTRef<DispNode> > dnTree;
extern	vector<UTRef<DispNode> > MakeDLTree();

void Display(void);
void Reshape(int width, int height);
void WriteScreenJPEG(const char* filename, int width, int height, int quality);

////////////////////////////////////////////////////////////////////////////////
//	GLUT callback functions

#define KEY_ESC -27
extern int		scWidth, scHeight;
static GLfloat	scale = 1;
static GLfloat	spinX = 0;
static GLfloat	spinY = 0;

static void MenuFunc(int key)
{
	switch (key) {
	case GLUT_KEY_UP:
		spinX -= 5;
		break;
	case GLUT_KEY_DOWN:
		spinX += 5;
		break;
	case GLUT_KEY_LEFT:
		spinY -= 5;
		break;
	case GLUT_KEY_RIGHT:
		spinY += 5;
		break;
	case GLUT_KEY_PAGE_UP:
		scale *= 1.05;
		break;
	case GLUT_KEY_PAGE_DOWN:
		scale /= 1.05;
		break;
	case GLUT_KEY_HOME:
		spinX = spinY = 0;
		scale = 1;
		break;
	case GLUT_KEY_F9:
		glutFullScreen();
		break;
	case GLUT_KEY_F12:
		{
			char	*tname = "snapshot.jpg";
			printf("Creating Screen Snapshot as '%s'.\n", tname);
			glutSetCursor(GLUT_CURSOR_WAIT);
			WriteScreenJPEG(tname, scWidth, scHeight, 95);
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		}
		break;
	case GLUT_KEY_F4:
	case KEY_ESC:
		exit(0);
		break;
	default:
		return;
	}
	glutPostRedisplay();
}

static void Key(unsigned char key, int x, int y)
{
	static	int		nPenSuffix = 0;

	switch(key){
	case 'a':
		{
			Affinef rotaf(Rad(30), 'y');
			*posPenguin = rotaf * ( *posPenguin );
			std::cout << (*posPenguin) << std::endl;

			Affinef rotaf2(Rad(30), 'x');
			vector<UTRef<DispNode> >::iterator itdnPen2;
			for( itdnPen2 = dnPen2.begin();
			itdnPen2 != dnPen2.end(); itdnPen2++ ){
				(*itdnPen2)->pos = rotaf2 * ( (*itdnPen2)->pos );
				std::cout << ( (*itdnPen2)->pos ) << std::endl;
			}
		}
	case 'b':
		glutPostRedisplay();
		break;
	case 'c':
		{
			UTRef<DispNode> dnTmp;
			nPenSuffix++;
			dnTmp = GetNodeMap()["Penguin"]->CopyNode(nPenSuffix);
			std::cout << "Create Pen" << nPenSuffix << std::endl;
			std::cout << ( dnTmp->pos ) << std::endl;
			dnTree.push_back( dnTmp );
			dnPen2.push_back( dnTmp );
		}
		break;
	case 'd':
		if( dnPen2.size() != 0 ){
			dnPen2.back()->DeleteCopy();
//			delete dnPen2.back();
			std::cout << "Delete Pen" << nPenSuffix << std::endl;
			nPenSuffix--;
			dnTree.pop_back();
			dnPen2.pop_back();
		}
		break;
	default:
		MenuFunc(-key);
	break;
	}
}

static void SpecialKey(int key, int x, int y)
{
	int	mdf = glutGetModifiers();
	if(mdf & GLUT_ACTIVE_CTRL){
		switch (key){
		case GLUT_KEY_UP:
			MenuFunc(GLUT_KEY_PAGE_UP);
			break;
		case GLUT_KEY_DOWN:
			MenuFunc(GLUT_KEY_PAGE_DOWN);
			break;
		}
	}else{
		MenuFunc(key);
	}
}

static int	mouseX, mouseY, mouseButton = -1;
static void Mouse(int button, int state, int x, int y)
{
	mouseButton = button;
	mouseX = x;	mouseY = y;
	if(state == GLUT_UP){
		mouseButton = -1;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case GLUT_LEFT_BUTTON:
		if(state != GLUT_UP)
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	case GLUT_MIDDLE_BUTTON:
		if(state != GLUT_UP)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	}
}

static void Motion(int x, int y)
{
	switch(mouseButton){
	case GLUT_LEFT_BUTTON:
		if(x == mouseX && y == mouseY)
			return;
		spinY += (x - mouseX);	spinX += (y - mouseY);
		glutPostRedisplay();
		break;
	case GLUT_MIDDLE_BUTTON:
		if(y == mouseY)
			return;
		if(y < mouseY)
			scale *= 1 + sqrt(mouseY - y)/20;
		else
			scale /= 1 + sqrt(y - mouseY)/20;
		glutPostRedisplay();
		break;
	}
	mouseX = x;	mouseY = y;
}

////////////////////////////////////////////////////////////////////////////////
//	initialize

static void Init(void)
{
	float	lmamb[] = {.7, .7, .7, 1},
			l0amb[] = {0, 0, 0, 1},
			l0dif[] = {1, 1, 1, 1},
			l0spc[] = {1, 1, 1, 1},
			l0pos[] = {0, 0, 10, 0};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);

	glutCreateMenu(MenuFunc);
	glutAddMenuEntry("Zoom In 	[PgUp]", GLUT_KEY_PAGE_UP);
	glutAddMenuEntry("Zoom Out	[PgDn]", GLUT_KEY_PAGE_DOWN);
	glutAddMenuEntry("Initialize ViewPoint	[Home]", GLUT_KEY_HOME);
	glutAddMenuEntry("------------------", 9999);
	glutAddMenuEntry("Full Screen	[F9]", GLUT_KEY_F9);
	glutAddMenuEntry("Screen Snapshot	[F12]", GLUT_KEY_F12);
	glutAddMenuEntry("------------------", 9999);
	glutAddMenuEntry("Quit  	[Esc]", GLUT_KEY_F4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void RotateView(void)
{
	glScalef(scale, scale, scale);
	glRotatef(spinX, 1, 0, 0);
	glRotatef(spinY, 0, 1, 0);
}

////////////////////////////////////////////////////////////////////////////////
//	main

int main(int argc, char *argv[])
{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
	fprintf(stderr, "wrl2view 1.3.6:       VRML2.0 File Viewer	Copyright (c)1999 Mamoru Shiroki\n");
	fprintf(stderr, "wrl2view++ 1.3.6+0.1: Copyright (c)2001 Modified by Shoichi Hasegawa\n");

	glutInitWindowSize(600, 450);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("wrl2view: VRML2.0 File Viewer");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);

	Init();

	if (argc <= 1) {
		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
		return 1;
	}
	UTRef<TSts> root = LoadWrl(argv[1]);
	InitList(root);

	// 床ノードの取り出し＆DL化
//	TNode* nField = NodeIDMap()["Field"];
//	UTRef<TSts> stsField = new TSts;
//	stsField->push_back(nField);
//	dlField = DrawList(stsField);

	// ペンギンノードの取り出し
//	TNode* nPenguin = NodeIDMap()["Penguin"];

	// ペンギンノードの座標取得(親ノードのtranslation取得)
//	TNode* nPenParent = nPenguin->GetParent();
//	std::cout << nPenParent->GetType()->GetName() << std::endl;
//	while( strcmp( nPenParent->GetType()->GetName(), "Transform" ) != 0 ){
//		nPenParent = nPenParent->GetParent();
//	}
//	float ftread[3];
//	CopyElm( nPenParent, "translation", ftread );
//	std::cout << ftread[0] << ftread[1] << ftread[2] << endl;
//	posPenguin.Pos() = (Vec3f) ftread;
//	std::cout << posPenguin << std::endl;

	// ペンギンノードのDL化
//	UTRef<TSts> stsPenguin = new TSts;
//	stsPenguin->push_back(nPenguin);
//	dlPenguin = DrawList(stsPenguin);

	dnTree = MakeDLTree();
	MakeDLs( dnTree );
	dnPen2.clear();
	posPenguin = &(GetNodeMap()["Penguin"]->pos);
	std::cout << (*posPenguin) << std::endl;

	glutMainLoop();

/*	int nPenSuffix = 0;
	UTRef<DispNode> dnTmp;
		{
			nPenSuffix++;
			dnTmp = GetNodeMap()["Penguin"]->CopyNode(nPenSuffix);
			std::cout << "Create Pen" << nPenSuffix << std::endl;
			std::cout << ( dnTmp->pos ) << std::endl;
			dnTree.push_back( dnTmp );
			dnPen2.push_back( dnTmp );
		}

		if( dnPen2.size != 0 ){
		std::cout << ((dnTmp->DeleteCopy())? "True" : "False") << std::endl;
			std::cout << "Delete Pen" << nPenSuffix << std::endl;
			nPenSuffix--;
			dnTree.pop_back();
			dnPen2.pop_back();
		} */

	return 0;
}
