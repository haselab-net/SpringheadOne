#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"
#include <HIS/HIStepWalk.h>
#include <HIS/HITurnTable.h>
#include "../Share/WalkThroughTools.h"


using namespace std;
using namespace LVRM;

class TGLView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	
	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;

	float statusOfUser, lastStatusOfUser;   //  ユーザの状態判定用変数
	float tableAngle, lastTableAngle;       //  ターンテーブルの角度データ

	//	衝突判定
	float radius;					//	アバターの半径
	TClosestPointDetectors cpds;	//	最近傍点の検出器
	TReflectNormalCalculator rnc;	//	反射面の法線の計算機
	TPenaltyCanceler penCancel;		//	ペナルティ解消の計算

	//	アバターの属性
	Affinef afEye;				//	アバターから見た目の位置
	Vec3f vel;					//	アバターの速度
	Vec3f dvel;					//	アバターの速度の変化
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;
public:
	Affinef afBody;				//	アバターの位置
	TGeoNodes allNodes;			//	すべてのノード
	Vec3f clearColor;			//	空の色
	bool bSender;				//	送信側ならば true
	CmdSocket cmdSocket;

	TGLView(bool bSender);
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	void OnJoystick(unsigned int button, int x, int y, int z);
	bool OnTimer();
	void OnIdle();
	void Step();
	void SetParam();
	bool LoadSettings();
	void InitCollisionDetector();
};

#endif

