#ifndef VR_REND_GLUTRENDER_H
#define VR_REND_GLUTRENDER_H
#include "LVRMRender/RDView.h"
#define GLUT_NO_LIB_PRAGMA

namespace Spr {

///	This opens and updates an OpenGL window.
class RDGlutView:public RDView{
	int winID;
public:
	RDGlutView();
	~RDGlutView();
	///	the render object
	static RDGlutView*& TheRender();
	//	operation
	virtual void Init(const char* window_title);		//	init and create window.
	virtual void Run();
	virtual void Exit();
	virtual void WindowPosition(int x, int y, int w, int h);
	virtual void Flip();

private:
	virtual void DoSetTimer(int id);
	static void EventHandlerTimer(int id);
	static void EventHandlerIdle();
	static void EventHandlerDraw();
	static void EventHandlerSize(int w, int h);
	static void EventHandlerClose(int i);
	static void EventHandlerMouse(int button, int state, int x, int y);
	static void EventHandlerKeyboard(unsigned char key, int x, int y);
	static void EventHandlerKeyboardUp(unsigned char key, int x, int y);
	static void EventHandlerSpecial(int key, int x, int y);
	static void EventHandlerSpecialUp(int key, int x, int y);
	static void EventHandlerMotion(int x, int y);
	static void EventHandlerPassiveMotion(int x, int y);
	static void EventHandlerJoystick(unsigned int button, int x, int y, int z);
};
}	//	namespace LVRM
 
#endif
