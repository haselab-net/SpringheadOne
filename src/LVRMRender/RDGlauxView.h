#ifndef VR_REND_GLAUXRENDER_H
#define VR_REND_GLAUXRENDER_H

#include "RDView.h"
#include <GL/glaux.h>

namespace Spr { 


///	This opens and updates an OpenGL window.
class RDGlauxView:public RDView{
public:
	static RDGlauxView*& TheRender();
	///	
	RDGlauxView();
	///	
	virtual ~RDGlauxView();
	//	operation
	///	Init and create window.
	virtual void Init(const char* window_title);
	///	Run main loop.
	virtual void Run();
	///	Exit from main loop.
	virtual void Exit();

	///	Set window position.
	virtual void WindowPosition(int x, int y, int w, int h);
	///	Flip (update) buffer.
	virtual void Flip();

private:
	static void CALLBACK EventHandlerIdle();
	static void CALLBACK EventHandlerDraw();
	static void CALLBACK EventHandlerSize(int w, int h);
	static void CALLBACK EventHandlerClose();
	static void CALLBACK EventHandlerMouseLeftUp(AUX_EVENTREC* rc);
	static void CALLBACK EventHandlerMouseLeftDown(AUX_EVENTREC* rc);
	static void CALLBACK EventHandlerMouseMiddleUp(AUX_EVENTREC* rc);
	static void CALLBACK EventHandlerMouseMiddleDown(AUX_EVENTREC* rc);
	static void CALLBACK EventHandlerMouseRightUp(AUX_EVENTREC* rc);
	static void CALLBACK EventHandlerMouseRightDown(AUX_EVENTREC* rc);
};

 
 }
#endif
