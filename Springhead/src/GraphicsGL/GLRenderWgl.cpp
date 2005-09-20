#include "GraphicsGL.h"
#pragma hdrstop

namespace Spr {;

SGOBJECTIMP(GLRenderWgl, GLRender);

GLRenderWgl::GLRenderWgl(){
	hWnd = NULL;
	hgl = NULL;
	hdc = NULL;
}
GLRenderWgl::~GLRenderWgl(){
	Release();
}
bool GLRenderWgl::Create(void* arg){
	hWnd = (HWND)arg;
	PIXELFORMATDESCRIPTOR pfd = { 
		  sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		  1,                     // version number 
		  PFD_DRAW_TO_WINDOW |   // support window 
		  PFD_SUPPORT_OPENGL |   // support OpenGL 
		  PFD_DOUBLEBUFFER,      // double buffered 
		  PFD_TYPE_RGBA,         // RGBA type 
		  24,                    // 24-bit color depth 
		  0, 0, 0, 0, 0, 0,      // color bits ignored 
		  0,                     // no alpha buffer 
		  0,                     // shift bit ignored 
		  0,                     // no accumulation buffer 
		  0, 0, 0, 0,            // accum bits ignored 
		  32,                    // 32-bit z-buffer 
		  0,                     // no stencil buffer 
		  0,                     // no auxiliary buffer 
		  PFD_MAIN_PLANE,        // main layer 
		  0,                     // reserved 
		  0, 0, 0                // layer masks ignored 
	}; 
	hdc = GetDC(hWnd);
    int pixelFormat=ChoosePixelFormat(hdc,&pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);	
	hgl = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hgl);
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	return true;
}
bool GLRenderWgl::SetViewport(void* arg){
	HWND hw = (HWND) arg;
	RECT rc;
	GetClientRect(hw, &rc);
	Resize(Vec2f(rc.right-rc.left, rc.bottom-rc.top));
	return true;
}
void GLRenderWgl::Release(){
	if (hgl){
		wglDeleteContext(hgl);
		hgl = NULL;
	}
	if (hWnd && hdc){
		ReleaseDC(hWnd, hdc);
		hWnd = NULL;
	}
}
void GLRenderWgl::Present(){
	SwapBuffers(hdc);
}

}	//	Spr


