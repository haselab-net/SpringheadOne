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
	int	mouseX, mouseY;
	float	scale;
	float	spinX;
	float	spinY;
	Button mouseButton;
	Affinef R;
	double setting[6];
	double viewNear,viewFar;//クリップの奥行き
	double fov_v;
	Affinef afCam,afProjector; //カメラの位置
	Affinef move;
public:
	Communication csSelect;

	MyView(){
		scale = 1;
		spinX=0;
		spinY=0;
		csSelect.view = this;
		LoadSettings();
		SetParam();
		viewNear = 1.0;
		viewFar = 10000.0;
		afCam.Pos() = Vec3f(0,500,0);
		move.Ez() = Vec3f(0,0,0.1);//歩幅
	}
	UTRef<class MyNode> dnTree;
	void SetParam();
	void LoadSettings();
	void OnDraw();

	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnSpecialKey(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	void RotateView();
	void setAfCam(Affinef af);
	Affinef refAfCam();
	void OnIdle();
};

#endif

