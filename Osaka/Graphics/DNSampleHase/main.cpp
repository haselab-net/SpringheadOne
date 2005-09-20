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
using namespace std;

typedef struct AIPenguin{
	Affinef afi;
}AIPenguin;

struct Game{
	float	FPS;
	AIPenguin pen1;
}game;

int	scWidth, scHeight;
UTRef<DispNode> dnTree;

Affinef afObj;

void WriteScreenJPEG(const char* filename, int width, int height, int quality);

#define KEY_ESC -27
static GLfloat	scale = 1;
static GLfloat	spinX = 90;/* カメラの初期角度（tilt）*/
//static GLfloat	spinY = 0;
static GLfloat	spinY = 180;/* カメラの初期角度（pan）*/

static const puckNum = 4;
static const penNum = 3;

////////////////////////////////////////////////////////////////////////////////
//	Game Management functions

class PenguinManager:public UTRefCount{
public:
	Penguin penguinGroup[penNum];
	Penguin workPenguin;
	UTRef<DispNode> penguin;
	void Action(int id);
	void ActionWing(int penguinId);
};
PenguinManager penguins;

void Character::AddChara(DispNode* child){
	workParent->AddChild(child);
}

void Character::DelChara(DispNode* child){
	child->DelFromParent();
}

void PenguinManager::Action(int id){
	static Affinef workAffine;
	workAffine = penguins.penguinGroup[id].penguinRoot->GetGlobalPosture();
	workAffine.Pos().X() = workAffine.Pos().X() + 1;
	penguins.penguinGroup[id].penguinRoot->SetGtoLPosture(workAffine);
}

void PenguinManager::ActionWing(int penId){
	static Affinef rWingAffin, lWingAffin;
	static int patapata;
	static int pata = 1;
	Vec3f theata1(Rad(0),Rad(0),Rad(-10)), distance(0,0,0), theata2(Rad(0),Rad(0),Rad(10));
	Matrix3f move1(theata1.norm(), theata1.unit());
	Matrix3f move2(theata2.norm(), theata2.unit());
	Matrix3f rRot = rWingAffin.Rot();
	Matrix3f lRot = lWingAffin.Rot();
	if(patapata % 5 == 0) pata *= -1;
	patapata += pata;
	if(pata < 0){
		rWingAffin.Rot() = move1 * rRot;
		lWingAffin.Rot() = move1 * lRot;
	}
	else{
		rWingAffin.Rot() = move2 * rRot;
		lWingAffin.Rot() = move2 * lRot;
	}
	rWingAffin.Pos() = distance;
	lWingAffin.Pos() = distance;
	penguinGroup[penId].rWing->SetPosture(rWingAffin);
	penguinGroup[penId].lWing->SetPosture(lWingAffin);
	cout << "penId is" << penId << endl;
}

class PuckManager:public UTRefCount{
public:	
	Puck spuck[puckNum];
	Puck bpuck[puckNum];
	UTRef<DispNode> puck;
	Puck workPuck;
};
PuckManager pucks;

////////////////////////////////////////////////////////////////////////////////
//	GLUT callback functions

class Application{
	static Application* theApp;
public:
	Application(){
		theApp = this;
	}
	static void DisplayEntry(void);
	void Display(void);
	static void ReshapeEntry(int, int);
	void Reshape(int width, int height);
	static void IdleEntry(void);
	void Idle(void);
	void RotateView(void);
};
Application* Application::theApp;

void Application::DisplayEntry(void){
	theApp->Display();
}

void Application::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	vieworient[0] = 0;
	vieworient[1] = -1;
	vieworient[2] = 0;
	vieworient[3] = 0;
	glRotatef(180/M_PI * vieworient[3], vieworient[0], vieworient[1], vieworient[2]);
	glTranslatef(-viewpos[0], -viewpos[1], -viewpos[2]);
	RotateView();

	dnTree->Draw();

	glutSwapBuffers();
}

void Application::ReshapeEntry(int w, int h){
	theApp->Reshape(w,h);
}

void Application::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(180/M_PI * fieldOfView, (GLfloat)width / height, .1, 10000);
	glMatrixMode(GL_MODELVIEW);

	scWidth = width; scHeight = height;
}

void Application::RotateView(void)
{
	glScalef(scale, scale, scale);
	glRotatef(spinX, 1, 0, 0);
	glRotatef(spinY, 0, 1, 0);
}

void Application::IdleEntry(void){
	theApp->Idle();
}

void Application::Idle(void)
{
//	penguins.ActionWing(0);
	game.pen1.afi = penguins.penguinGroup[0].penguinRoot->GetPosture();
//	cout << game.FPS++ << endl;
//	cout << penguins.penguinGroup[0].penguinRoot->GetGlobalPosture() << endl;
//	cout << pucks.bpuck[0].puckRoot->GetGlobalPosture() << endl;
	glutPostRedisplay();
}

Application appli;

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

void Move(int penId, Affinef pen_move)
{
	penguins.penguinGroup[penId].penguinRoot->SetPosture(pen_move);
}

static void Key(unsigned char key, int x, int y)
{
	static	int		nPenSuffix = 0;
	Vec3f addPos(0,0,-5);
	Vec3f theata1(Rad(1),Rad(1),Rad(1));
	Matrix3f move1(theata1.norm(), theata1.unit());
	afObj.Rot() = move1;
	afObj.Pos() = addPos;
	cout << afObj << endl;
	switch(key){
	case 'm':{
			 }
		break;
	case 'a':{
		penguins.penguinGroup[0].AddChara(penguins.penguinGroup[0].penguinRoot);
		pucks.bpuck[0].AddChara(pucks.bpuck[0].puckRoot);
		pucks.spuck[0].AddChara(pucks.spuck[0].puckRoot);
			 }
		break;
	case 'd':{
		penguins.penguinGroup[0].DelChara(penguins.penguinGroup[0].penguinRoot);
		pucks.bpuck[0].DelChara(pucks.bpuck[0].puckRoot);
		pucks.spuck[0].DelChara(pucks.spuck[0].puckRoot);
			 }
		break;
	case 'p':{
		dnTree->Dump(std::cout);
		}
		break;
	case 't':{
		cout << penguins.penguinGroup[0].penguinRoot->GetGlobalPosture() << endl;
			 }
		break;
	case 's':{
		penguins.penguinGroup[0].penguinRoot->SetGtoLPosture(afObj);
			 }
		break;
	case 'w':{
		penguins.ActionWing(0);
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

	//Game Initialize
	game.FPS = 0.0f;
	game.pen1;
}


void InitArrange(void)
{
	penguins.penguin = DispNode::NodeMap()["PFrame"];
	if (penguins.penguin){
		penguins.workPenguin.workParent = penguins.penguin->GetParent();
		penguins.penguin->DelFromParent();
		for(int i = 0; i < penNum; i++){
			penguins.workPenguin.characterId = i;
			penguins.workPenguin.penguinRoot = penguins.penguin->Copy(i);
			penguins.workPenguin.rWing = penguins.workPenguin.penguinRoot->Children().front()->Children().front()->Children().front();
			penguins.workPenguin.lWing = penguins.workPenguin.penguinRoot->Children().front()->Children().front()->Children().at(1);
			penguins.penguinGroup[i] = penguins.workPenguin;
		}
	}
	else
		cout << "no Penguins" << endl;
	
	pucks.puck = DispNode::NodeMap()["SPFrame"];
	if(pucks.puck){
		pucks.workPuck.workParent = pucks.puck->GetParent();
		pucks.puck->DelFromParent();
		for(int i = 0; i < puckNum; i++){
			pucks.workPuck.characterId = i;
			pucks.workPuck.puckRoot = pucks.puck->Copy(i);
			pucks.spuck[i] = pucks.workPuck;
		}
	}
	else
		cout << "no SmallPuck" << endl;

	pucks.puck = DispNode::NodeMap()["BPFrame"];
	if(pucks.puck){
		pucks.workPuck.workParent = pucks.puck->GetParent();
		pucks.puck->DelFromParent();
		for(int i = 0; i < puckNum; i++){
			pucks.workPuck.characterId = i;
			pucks.workPuck.puckRoot = pucks.puck->Copy(i);
			pucks.bpuck[i] = pucks.workPuck;
		}
	}
	else
		cout << "no BigPuck" << endl;
}

////////////////////////////////////////////////////////////////////////////////
//	main

int main(int argc, char *argv[])
{
	fprintf(stderr, "wrl2view 1.3.6:       VRML2.0 File Viewer	Copyright (c)1999 Mamoru Shiroki\n");
	fprintf(stderr, "wrl2view++ 1.3.6+0.1: Copyright (c)2001 Modified by Shoichi Hasegawa\n");

	glutInitWindowSize(600, 450);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("wrl2view: VRML2.0 File Viewer");

	glutDisplayFunc(appli.DisplayEntry);
	glutReshapeFunc(appli.ReshapeEntry);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(appli.IdleEntry);
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);

	Init();

	if (argc <= 1) {
		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
		return 1;
	}
	UTRef<TSts> root = LoadWrl(argv[1]);
	InitList(root);

	DispNodeBuilder builder;
	builder.Build(root);
	InitArrange();
	dnTree = builder.Root();

	glutMainLoop();

	return 0;
}

