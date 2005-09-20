#ifndef VR_REND_GLUTRENDER_H
#define VR_REND_GLUTRENDER_H
#include "RDView.h"

namespace VRRend{
using namespace VRBase;

//
//	RDGlutView  --  display class
//
//	This opens and updates an OpenGL window.
//
class RDGlutView:public RDView{
	int winID;
public:
	RDGlutView();
	~RDGlutView();
	//	the render object
	static RDGlutView*& TheRender();
	//	operation
	virtual void Init(const char* window_title);		//	init and create window.
	virtual void Run();
	virtual void Exit();
	virtual void WindowPosition(int x, int y, int w, int h);
	virtual void Flip();

private:
	static void EventHandlerIdle();
	static void EventHandlerDraw();
	static void EventHandlerSize(int w, int h);
	static void EventHandlerClose();
	static void EventHandlerMouse(int button, int state, int x, int y);
	static void EventHandlerKeyboard(unsigned char key, int x, int y);
	static void EventHandlerSpecialKey(int key, int x, int y);
};

}

#endif
