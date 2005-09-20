#ifndef TGLView_H
#define TGLView_H

#include "MyNode.h"

#include <LVRMRender/RDGlutView.h>
#include <vector>

using namespace std;
using namespace Spr;

class TGLView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;

public:
	Affinef afCam;
	float fov;
	TMyNodes allNodes;
	Vec3f clearColor;			//	ãÛÇÃêF
	
	TGLView();
	void OnDraw();
	void OnSize(int width, int height);
	void OnSpecialKey(int key, int x, int y);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
};

#endif

