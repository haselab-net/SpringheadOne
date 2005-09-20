#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"

using namespace std;
using namespace LVRM;

class TGLView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;

	//	衝突判定
	float radius;				//	アバターの半径
	TPenaltyCheckers penChekers;//	ペナルティーチェッカー
	std::vector<Vec3d> normals;	//	衝突面の法線ベクトルたち
	Vec3d reflect;				//	速度の反射ベクトル．
	//	アバターの属性
	Affinef afEye;				//	アバターから見た目の位置
	Vec3f vel;					//	アバターの速度
	Vec3f dvel;					//	アバターの速度の変化
	
	bool bDrawNormal;
	bool bDrawCollision;
public:
	Affinef afBody;				//	アバターの位置\begin{abstract}\end{abstract}
	float aspect_x,aspect_y;
	TGeoNodes allNodes;
	Vec3f clearColor;			//	空の色
	bool bSender;
	CmdSocket cmdSocket;

	TGLView();
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	bool OnTimer();
	void Step();
	void SetParam();
	bool LoadSettings();
	void InitCollisionDetector();
};

#endif

