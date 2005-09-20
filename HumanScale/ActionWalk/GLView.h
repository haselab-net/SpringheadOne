#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"

//#include "HIS/HIStepWalk.h"
//#include "HIS/HITurnTable.h"
//#include "../Share/WalkThroughTools.h"
//#include "LabelingCamera.h"
//#include "Stereo.h"
#define PC_NUM 24

using namespace std;
using namespace LVRM;

#include <HIS/HISpidar4Quad.h>
#include <Device/DRUsb20Simple.h>

class TGLView: public RDGlutView{
	int mouseX, mouseY;
//	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	float lastTableAngle;
	
	//	アバターの属性
	Affinef afEye;				//	アバターから見た目の位置
	bool bFalling;				//	アバター落下中フラグ
	float fallingTime;			//  アバター落下時間
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;
    DVDeviceManager devMan;
   // HISpidar4quad kk[2];

public:
	
	float radius;					//	アバターの半径
	float hand_radius;	
    
	Affinef afLHand;			//  アバターの左手の位置　追加：赤羽
	Affinef afRHand;			//  アバターの右手の位置　追加：赤羽

	Affinef afBody;				//	アバターの位置
	Affinef afInitialBody;		//  アバターの初期位置
	Vec3f VecMyself[3];			//  自分自身の位置
	TGeoNodes allNodes;			//	すべてのノード
	Vec3f clearColor;			//	空の色
	bool bSender;				//	送信側ならば true
	int viewMode;				//  視点のモード (0:アパタ 1:上から視点 2:鳥瞰視点)
	int viewTurn;				//  視点の回転角
	float topHeight;			//  上から視点の高さ
	float topShift;				//  上から視点の中心からのずれ
	float birdsHeight;			//  鳥瞰視点の高さ
	float birdsRadius;			//  鳥瞰視点の角度
	float birdsShift;			//  鳥瞰視点の中心からのずれ
	float cRadius;				//  半径
	int cSides;				//  角の数
	double PI2;				//  円周角
	float cStep;				//

	Vec3f VecCircle[32];
	Vec3f VecSquare[4];
	Vec3f VecSensor[4];
	Vec3f VecStartPosition[4];

	int nowMode;
	CmdSocket cmdSocket;
	TGLView(int nowMode);
	GLdouble bEye[3], bTarget[3], bUp[3];
	int cpNum;					// PC名識別用
	int SyncBuf[PC_NUM];
   
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnSpecialKey(int key, int x, int y); 

	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	void OnJoystick(unsigned int button, int x, int y, int z);
	bool OnTimer();
	void OnIdle();
	void Step();
	void SetParam();
	bool LoadSettings();

	void conn_motion(int init_type, double k_time);
	void drawBody(int cur_frame);
//	void drawjoint(int cur_frm, MyBone sk_bone);
    void ballColliding(int cur_frm);
    void handColliding();
	void init_ball();
	void init_paddle();
	void init_hand();

	std::ofstream *centerGFile;
	int printFileNameTime;
	char printFileName[16];

	std::ofstream *turnDataFile;
	int printTurnDataTime;
	char printTurnData[16];

	char* folderName;
	char folderNameBack[20];

	void PrintHz(std::ostream& os) const;
	int stepHz;
	std::ofstream *printStepHz;

protected:
	DWORD dwLastTime;
	DWORD dwTime;

	int statusOfUser;
	int lastStatusOfUser;

	Vec3f labelP[2];

};

#endif

