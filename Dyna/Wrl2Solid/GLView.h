#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>

using namespace std;
using namespace LVRM;

class TGLView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;
	Vec3f probe;
	float radius;
	TPenaltyCheckers pcs;
public:
	TGeoNodes allNodes;
	Affinef afCam;	//ÉJÉÅÉâÇÃà íu
	Vec3f clearColor;
	
	TGLView();
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnSpecialKey(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	void OnIdle();
	
	void InitCollisionDetector();
	void HapticStep();
};

#endif

