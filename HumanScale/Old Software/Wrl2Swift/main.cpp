// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "GeoNode.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <SWIFT.h>
#include <strstream>


int	scWidth, scHeight;

void Display(void);
void Reshape(int width, int height);

////////////////////////////////////////////////////////////////////////////////
//	GLUT callback functions

#define KEY_ESC -27
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
	case GLUT_KEY_F4:
	case KEY_ESC:
		exit(0);
		break;
	default:
		return;
	}
	glutPostRedisplay();
}

void RotateView(void);

////////////////////////////////////////////////////////////////////////////////
//	GLUT callback functions

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(180/M_PI * vieworient[3], vieworient[0], vieworient[1], vieworient[2]);
	glTranslatef(-viewpos[0], -viewpos[1], -viewpos[2]);
	RotateView();

	for(TGeoNode::TGeoNodes::iterator it = TGeoNode::allNodes.begin(); it != TGeoNode::allNodes.end(); ++it){
//		(*it)->Draw();
		(*it)->DrawGeo();
	}
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



static void Key(unsigned char key, int x, int y)
{
	static	int		nPenSuffix = 0;

	switch(key){
	case 'b':
		glutPostRedisplay();
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

void PrintPoly(){
	TGeoNode* curNode;
	for(TGeoNode::TGeoNodes::iterator it = TGeoNode::allNodes.begin(); it != TGeoNode::allNodes.end(); ++it){
		curNode = *it;

		string exPoly(".poly");
		string filename = curNode->Name() + exPoly;
		cout << filename << endl;
		cout << curNode->vertices.size() << endl;
		cout << curNode->faces.size() << endl;
		if(curNode->vertices.size() != 0 || curNode->faces.size() != 0){
			ofstream polyout(filename.c_str());
			polyout << "POLY" << endl;
			polyout << endl;
			polyout << curNode->vertices.size() << endl;
			polyout << endl;
			polyout << curNode->faces.size() << endl;
			polyout << endl;
			for(TGeoNode::TVertices::iterator vertice = curNode->vertices.begin(); vertice != curNode->vertices.end(); ++vertice){
				polyout << vertice->X() << ' ' << vertice->Y() << ' ' << vertice->Z() <<endl;
			}
			polyout << endl;
			for(TGeoNode::TFaces::iterator face = curNode->faces.begin(); face != curNode->faces.end(); ++face){
				polyout << "3" << ' ';
				for(vector<int>::iterator point = face->begin(); point != face->end(); ++point){
					polyout << *point << ' ';
				}
				polyout << endl;
			}
			polyout.close();
		}
	}
}

void Decomposer(){
	TGeoNode* curNode;
	string decomposer("..\\..\\SWIFT++\\decomposer\\Debug\\decomposer_c ");
	string pass("..\\..\\PIOpen\\Wrl2Swift\\");
	string jAmpl("-j 0.000001 ");
	string eErr("-e 0.0001 ");
	string ef("-ef ");
	string l("-l ");
	string dfs("-dfs ");
	string bfs("-bfs ");
	string cbfs("-cbfs ");
	string df("-df ");
	string hier("-hier ");
	string hf("-hf ");
	string sSplit("-s split ");
	string exPoly(".poly");
	string exDcp(".dcp");
	string exChr(".chr");
	string sp(" ");
	
	for(TGeoNode::TGeoNodes::iterator it = TGeoNode::allNodes.begin(); it != TGeoNode::allNodes.end(); ++it){
		string polyFile,dcpFile,chrFile;
		string order;

		curNode = *it;
		if(curNode->vertices.size() != 0 || curNode->faces.size() != 0){
			polyFile = curNode->Name() + exPoly;
			dcpFile = curNode->Name() + exDcp;
			chrFile = curNode->Name() + exChr;
			order = decomposer + jAmpl + eErr + bfs + hier + hf + chrFile + sp + pass + polyFile;

			system(order.c_str());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//	main

int main(int argc, char *argv[])
{
//	ÉÅÉÇÉäÉäÅ[ÉNåüç∏óp
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
	std::cout << "Loaded\n";
	InitList(root);
	std::cout << "Initialized\n";

	TGeoNodeBuilder builder;
	builder.Build(root);
	std::cout << "Builded\n";

	PrintPoly();
	
	Decomposer();

	glutMainLoop();

	return 0;
}
