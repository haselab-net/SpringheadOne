#include "GraphicsGL.h"
#include "gl/glut.h"
#pragma hdrstop

namespace Spr {;

SGOBJECTIMP(GLRenderGlut, GLRender);

GLRenderGlut::GLRenderGlut(){
}
GLRenderGlut::~GLRenderGlut(){
	Release();
}
bool GLRenderGlut::Create(void* arg){
	int argc = 1;
	char* argv="";
	glutInit(&argc, &argv);

	glutInitDisplayString("rgba double depth~32"); 
  	winID = __glutCreateWindowWithExit(window_title, EventHandlerClose);


	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	return true;
}
bool GLRenderGlut::SetViewport(void* arg){

	
	HWND hw = (HWND) arg;
	RECT rc;
	GetClientRect(hw, &rc);
	Resize(Vec2f(rc.right-rc.left, rc.bottom-rc.top));
	return true;
}

void GLRenderGlut::Release(){
}
void GLRenderGlut::Present(){
	SwapBuffers(hdc);
}

}	//	Spr


