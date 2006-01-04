#include "MyNode.h"
#include "GLView.h"
#include <GL/glut.h>
#include <crtdbg.h>
#include <strstream>

using namespace std;

#define KEY_ESC -27
const float floorHeight = 0.0f;
const float dt = 1.0f / 30.0f;
const float gravity = 9.8f;
const float elasticity = 0.5f;

TGLView::TGLView(){
	fov = Rad(45.0f);
}
void TGLView::OnDraw(){
	glClearColor(clearColor.X(), clearColor.Y(), clearColor.Z(), 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Affinef afView = afCam.inv();
	glLoadIdentity();
	glLoadMatrixf(afView);
	allNodes.ZSort(afView);
	allNodes.Draw();
	Flip();
}

void TGLView::OnSize(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(Deg(fov) , (float)width/(float)height , 2.0f , 1000.0f);
//	gluPerspective(Deg(fov) , (float)width/(float)height , 10.0f , 100000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	float delta = 0.5f;
	switch(key){
	case '\x1b':
		Exit();
		break;
	case 'b':
		glutPostRedisplay();
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'w':
		glutReshapeWindow(400,300);
		break;
	default:
		break;
	}
}
void TGLView::OnSpecialKey(int key, int x, int y)
{
	float delta = 0.5f;
	switch(key){
	case GLUT_KEY_UP:
		afCam.Pos() -= afCam.Ez()*delta;
		Invalidate();
		break;
	case GLUT_KEY_DOWN:
		afCam.Pos() += afCam.Ez()*delta;
		Invalidate();
		break;
	case GLUT_KEY_HOME:
		afCam.Pos() -= afCam.Ey()*delta;
		Invalidate();
		break;
	case GLUT_KEY_END:
		afCam.Pos() += afCam.Ey()*delta;
		Invalidate();
		break;
	case GLUT_KEY_LEFT:
		afCam.Rot() =  afCam.Rot() * Affinef::Rot(Rad(5), 'y').Rot();
		Invalidate();
		break;
	case GLUT_KEY_RIGHT:
		afCam.Rot() =  afCam.Rot() * Affinef::Rot(Rad(-5), 'y').Rot();
		Invalidate();
		break;
	case GLUT_KEY_PAGE_UP:
		afCam.Rot() =  afCam.Rot() * Affinef::Rot(Rad(-5), 'x').Rot();
		Invalidate();
		break;
	case GLUT_KEY_PAGE_DOWN:
		afCam.Rot() = afCam.Rot() * Affinef::Rot(Rad(5), 'x').Rot();
		Invalidate();
		break;
	default:
		break;
	}
}

void TGLView::OnMouse(Button button, bool on, int x, int y)
{
	mouseButton = button;
	mouseX = x;	mouseY = y;
	if(!on){
		mouseButton = NONE;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case LEFT:
		if(on)
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	case MIDDLE:
		if(on)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	}
}

void TGLView::OnMouseMove(bool on, int x, int y)
{
	switch(mouseButton){
	case LEFT:
		Invalidate();
		break;		
	case MIDDLE:
		Invalidate();
		break;
	case RIGHT:
		Invalidate();
		break;
	}
	mouseX = x;	mouseY = y;
}
void TGLView::OnOpen(void){
	//	GLÇÃèâä˙âª
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
}
