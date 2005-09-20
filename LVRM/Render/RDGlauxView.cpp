#include "RDGlauxView.h"
#include <GL/glaux.h>
#include <assert.h>
#include <stdio.h>

namespace LVRM {

static void GLCheckError (){
	GLenum err = glGetError ();		// Get error status
	if (err == GL_NO_ERROR) return;	// normal case
	fprintf (stderr, "OpenGL error: %s\n", gluErrorString (err));	// msg

	assert (err == GL_NO_ERROR);	// will trap 
	exit (1);						// fails 
}

static RDGlauxView* theRender;
RDGlauxView*& RDGlauxView::TheRender(){
	return theRender;
}
void CALLBACK RDGlauxView::EventHandlerIdle(){
	if (TheRender()){
		if (TheRender()->invalidated) TheRender()->OnDraw();
		TheRender()->OnIdle();
	}
}
void CALLBACK RDGlauxView::EventHandlerDraw(){
	if (TheRender()){
		TheRender()->OnDraw();
		TheRender()->Validate();
	}
}
void CALLBACK RDGlauxView::EventHandlerSize(int w, int h){
	if (TheRender()) TheRender()->OnSize(w, h);
}
void CALLBACK RDGlauxView::EventHandlerClose(){
	if (TheRender()) TheRender()->OnClose();
}
void CALLBACK RDGlauxView::EventHandlerMouseLeftDown(AUX_EVENTREC* rc){
	if (TheRender()) TheRender()->OnMouse(LEFT, true, rc->data[AUX_MOUSEX], rc->data[AUX_MOUSEY]);
}
void CALLBACK RDGlauxView::EventHandlerMouseLeftUp(AUX_EVENTREC* rc){
	if (TheRender()) TheRender()->OnMouse(LEFT, false, rc->data[AUX_MOUSEX], rc->data[AUX_MOUSEY]);
}
void CALLBACK RDGlauxView::EventHandlerMouseMiddleDown(AUX_EVENTREC* rc){
	if (TheRender()) TheRender()->OnMouse(MIDDLE, true, rc->data[AUX_MOUSEX], rc->data[AUX_MOUSEY]);
}
void CALLBACK RDGlauxView::EventHandlerMouseMiddleUp(AUX_EVENTREC* rc){
	if (TheRender()) TheRender()->OnMouse(MIDDLE, false, rc->data[AUX_MOUSEX], rc->data[AUX_MOUSEY]);
}
void CALLBACK RDGlauxView::EventHandlerMouseRightDown(AUX_EVENTREC* rc){
	if (TheRender()) TheRender()->OnMouse(RIGHT, true, rc->data[AUX_MOUSEX], rc->data[AUX_MOUSEY]);
}
void CALLBACK RDGlauxView::EventHandlerMouseRightUp(AUX_EVENTREC* rc){
	if (TheRender()) TheRender()->OnMouse(RIGHT, false, rc->data[AUX_MOUSEX], rc->data[AUX_MOUSEY]);
}
RDGlauxView::RDGlauxView(){
	assert(!TheRender());
	TheRender() = this;
}
RDGlauxView::~RDGlauxView(){
	assert(TheRender());
	TheRender() = NULL;
}
void RDGlauxView::WindowPosition(int x, int y, int w, int h){
	auxInitPosition(x, y, w, h);
	glViewport(0, 0, w, h);
}
void RDGlauxView::Init(const char* window_title){
	auxInitDisplayMode(AUX_RGBA | AUX_DOUBLE);
  	auxInitWindow(window_title);
	glLineWidth (2.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}
void RDGlauxView::Run(){
	auxReshapeFunc(EventHandlerSize);
	auxIdleFunc(EventHandlerIdle);
	auxMouseFunc(AUX_LEFTBUTTON,	AUX_MOUSEDOWN,	EventHandlerMouseLeftDown);
	auxMouseFunc(AUX_LEFTBUTTON,	AUX_MOUSEUP,	EventHandlerMouseLeftUp);
	auxMouseFunc(AUX_MIDDLEBUTTON,	AUX_MOUSEDOWN,	EventHandlerMouseMiddleDown);
	auxMouseFunc(AUX_MIDDLEBUTTON,	AUX_MOUSEUP,	EventHandlerMouseMiddleUp);
	auxMouseFunc(AUX_RIGHTBUTTON,	AUX_MOUSEDOWN,	EventHandlerMouseRightDown);
	auxMouseFunc(AUX_RIGHTBUTTON,	AUX_MOUSEUP,	EventHandlerMouseRightUp);
	auxMainLoop(EventHandlerDraw);
}
void RDGlauxView::Flip(){
	glFlush();
	auxSwapBuffers();
}

void RDGlauxView::Exit(){
}

}	//	namespace LVRM
