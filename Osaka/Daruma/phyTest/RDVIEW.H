#ifndef VR_REND_RENDER_H
#define VR_REND_RENDER_H
#include <Base/Affine.h>

NAMESPACE_START(VRRend)

using namespace VRBase;
//
//	RDView  --  display class
//
class RDView{
protected:
	bool invalidated;
public:
	RDView();
	virtual ~RDView();
	//	operation
	///	Init and create window.
	virtual void Init(const char* window_title)=0;
	///	Run main loop.
	virtual void Run()=0;
	///	Exit from main loop.
	virtual void Exit()=0;

	///	Set window position.
	virtual void WindowPosition(int x, int y, int w, int h)=0;
	///	Flip (update) buffer.
	virtual void Flip()=0;
	///	Request rendering.
	void Invalidate(){ invalidated = true; }
	///	Cancel the request of rendering.
	void Validate(){invalidated = false; }

public:
	//	Call backs.
	///	
	virtual void OnDraw(){}
	///	
	virtual void OnIdle(){}
	///	
	virtual void OnSize(int w, int h){}
	///	
	virtual void OnOpen(){}
	///	
	virtual void OnClose(){}
	///	
	enum Button {NONE, LEFT, MIDDLE, RIGHT};
	///
	virtual void OnMouse(Button b, bool on, int x, int y){}
	///
	virtual void OnKeyboard(int code, int x, int y){}
};

NAMESPACE_END(VRRend)
#endif
