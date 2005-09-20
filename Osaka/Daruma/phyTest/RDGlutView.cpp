#include "RDGlutView.h"
#include <stdio.h>
#ifdef __BORLANDC__
#define GLUT_BUILDING_LIB
#endif
#include <gl/glut.h>
#include <windows.h>
#include <assert.h>

namespace VRRend{

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
void RDGlutView::EventHandlerIdle(){
	if (theRender){
		if (theRender->invalidated) theRender->OnDraw();
		theRender->OnIdle();
	}
}
void RDGlutView::EventHandlerDraw(){
	if (theRender){
		theRender->OnDraw();
		theRender->Validate();
	}
}
void RDGlutView::EventHandlerSize(int w, int h){
	if (theRender) theRender->OnSize(w, h);
}
void RDGlutView::EventHandlerClose(){
	if (theRender) theRender->OnClose();
}
void RDGlutView::EventHandlerMouse(int button, int state, int x, int y){
	if (theRender) theRender->OnMouse((RDGlutView::Button)button, state==0, x, y);
}
void RDGlutView::EventHandlerKeyboard(unsigned char key, int x, int y){
	if (theRender) theRender->OnKeyboard(key, x, y);
}
void RDGlutView::EventHandlerSpecialKey(int key, int x, int y){
	if (theRender) theRender->OnKeyboard(-key, x, y);
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
  	winID = glutCreateWindow(window_title);
	OnOpen();
	glLineWidth (2.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	float amb[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}
void RDGlutView::Run(){
	glutReshapeFunc(EventHandlerSize);
	glutIdleFunc(EventHandlerIdle);
	glutMouseFunc(EventHandlerMouse);
	glutKeyboardFunc(EventHandlerKeyboard);
	glutSpecialFunc(EventHandlerSpecialKey);
	glutDisplayFunc(EventHandlerDraw);
	glutMainLoop();
}
void RDGlutView::Flip(){
	glFlush();
	glutSwapBuffers();
}

void RDGlutView::Exit(){
	exit(0);
}

}
