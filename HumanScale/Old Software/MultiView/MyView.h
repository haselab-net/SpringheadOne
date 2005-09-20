#ifndef MYVIEW_H
#define MYVIEW_H

#include <Render/RDGlutView.h>
#include "communication.h"
#include "Base/Affine.h"
#include "Base/TMatrix.h"
#include "Base/TMatrixUtility.h"
using namespace std;
using namespace LVRM;

class MyView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;
	Affinef scale;
	Affinef R;
	double setting[6];
	double viewNear,viewFar;//クリップの奥行き
	double fov_v;

public:
	Affinef afCam,afProjector; //カメラの位置
	Affinef move;
	Affinef eye;
	float aspect_x,aspect_y;
	bool bSender;
	class MyNode* dnTree;
	CmdSocket cmdSocket;

	MyView();
	void SetParam();
	bool LoadSettings();
	void OnDraw();

	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnSpecialKey(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	void OnIdle();
	void print_performance();
};

#endif

