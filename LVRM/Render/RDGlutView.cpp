#include "RDGlutView.h"
#include <stdio.h>
#define GLUT_BUILDING_LIB
#include <gl/glut.h>
#include <windows.h>
#include <assert.h>

namespace LVRM {

static void GLCheckError (){
	GLenum err = glGetError ();		// Get error status
	if (err == GL_NO_ERROR) return;	// normal case
	fprintf (stderr, "OpenGL error: %s\n", gluErrorString (err));	// msg

	assert (err == GL_NO_ERROR);	// will trap 
	exit (1);						// fails 
}
static RDGlutView* theRender;
RDGlutView*& RDGlutView::TheRender(){
	return theRender;
}

void RDGlutView::DoSetTimer(int id){
	glutTimerFunc(timerHandlers[id]->interval, EventHandlerTimer, id);
}
void RDGlutView::EventHandlerTimer(int id){
	if (theRender){
		if (theRender->timerHandlers[id]){
			glutTimerFunc(theRender->timerHandlers[id]->interval, EventHandlerTimer, id);
			if (!theRender->timerHandlers[id]->Call()){
				theRender->timerHandlers.Del(id);
			}
		}
	}
}
void RDGlutView::EventHandlerIdle(){
	if (theRender){
		if (theRender->invalidated){
			theRender->OnDraw();
			theRender->Validate();
		}
		theRender->OnIdle();
	}
}
void RDGlutView::EventHandlerDraw(){
	if (theRender){
		theRender->Validate();
		theRender->OnDraw();
	}
}
void RDGlutView::EventHandlerSize(int w, int h){
	if (theRender) theRender->OnSize(w, h);
}
void RDGlutView::EventHandlerClose(int i){
	if (theRender) theRender->OnClose();
}
void RDGlutView::EventHandlerMouse(int button, int state, int x, int y){
	Button map[] = {LEFT, MIDDLE, RIGHT, NONE};
	if (theRender) theRender->OnMouse(map[button], state==0, x, y);
}
void RDGlutView::EventHandlerMotion(int x, int y){
	if (theRender) theRender->OnMouseMove(true, x, y);
}
void RDGlutView::EventHandlerPassiveMotion(int x, int y){
	if (theRender) theRender->OnMouseMove(false, x, y);
}
void RDGlutView::EventHandlerKeyboard(unsigned char key, int x, int y){
	if (theRender){
		theRender->OnKeyboard(key, x, y);
		theRender->keyboard[key]++;
	}
}
void RDGlutView::EventHandlerKeyboardUp(unsigned char key, int x, int y){
	if (theRender){
		theRender->keyboard[key]=0;
	}
}
void RDGlutView::EventHandlerSpecial(int key, int x, int y){
	if (theRender){
		theRender->OnSpecialKey(key, x, y);
		if (key<sizeof(theRender->specialKey)/sizeof(theRender->specialKey[0])){
			theRender->specialKey[key]++;
		}
	}
}
void RDGlutView::EventHandlerSpecialUp(int key, int x, int y){
	if (theRender){
		if (key<sizeof(theRender->specialKey)/sizeof(theRender->specialKey[0])){
			theRender->specialKey[key]=0;
		}
	}
}
void RDGlutView::EventHandlerJoystick(unsigned int button, int x, int y, int z){
	if (theRender){
		theRender->OnJoystick(button, x, y, z);
	}
}
RDGlutView::RDGlutView(){
	assert(!theRender);
	theRender = this;
	winID = 0;
}
RDGlutView::~RDGlutView(){
	assert(theRender);
	theRender = NULL;
}
void RDGlutView::WindowPosition(int x, int y, int w, int h){
	glutInitWindowPosition(x, y);
	glutInitWindowSize(w, h);
	glViewport(0, 0, w, h);
}
void RDGlutView::Init(const char* window_title){
	int argc = 1;
	char* argv="";
	glutInit(&argc, &argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
  	winID = __glutCreateWindowWithExit(window_title, EventHandlerClose);

	glLineWidth (2.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	float amb[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glEnable(GL_CULL_FACE);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}
void RDGlutView::Run(){
	glutReshapeFunc(EventHandlerSize);
	glutIdleFunc(EventHandlerIdle);
	glutMouseFunc(EventHandlerMouse);
	glutKeyboardFunc(EventHandlerKeyboard);
	glutKeyboardUpFunc(EventHandlerKeyboardUp);
	glutSpecialFunc(EventHandlerSpecial);
	glutSpecialUpFunc(EventHandlerSpecialUp);
	glutDisplayFunc(EventHandlerDraw);
	glutMotionFunc(EventHandlerMotion);
	glutPassiveMotionFunc(EventHandlerPassiveMotion);
	glutJoystickFunc(EventHandlerJoystick, 30);
	OnOpen();
	glutMainLoop();
}
void RDGlutView::Flip(){
	glFlush();
	glutSwapBuffers();
}

void RDGlutView::Exit(){
	OnClose();
	exit(0);
}

}	//	namespace LVRM