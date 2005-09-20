#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"

//SOUND
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

//SLCamera
#include "Camera\LabelingCamera.h"
//~SLCamera
///////////////////////
#include "direxec.h"
//#include "joint.h"
////////////////////////
using namespace std;
using namespace LVRM;
//SPIDAR-H
/////////////////////////////////////////////
#include <His/HISpidar4Quad.h>
#include <Device/DRUsb20Simple.h>
// 動作確認SPIDAR用 モータの取り付け座標
#define PX	0.3125f			//	x方向の辺の長さ/2 [m]
#define PY	0.23f			//	y方向の辺の長さ/2 [m]
#define PZ	0.265f			//	z方向の辺の長さ/2 [m]
#define MAXON	0.3776f
#define LPP	2.924062107079e-5f
const float K = 3;
#define SpidarUpdateRate 2
#define BallSpeedThreshold 1.0 * convert_inch
/////////////////////////////////////////////
#define PC_NUM 24

//SLCamera 10/21
#define BallLogMax 33

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

	//SLCamear
	////////////////////////////////
	//10/21
	Vec3f BallDirection;
	int MainAngleSide;
	int MainAngleVert;
	int UserAngleSide;
	int UserAngleVert;
	int BallAngleSide;
	int BallAngleVert;
	Vec3f BallPosChange[BallLogMax];
	Vec3f BallPosLog2[BallLogMax];
	Vec3f BallPosChangeTotal;
	//~10/21

	//MPI031021
	#define MOTION_DAT_BUF_SIZE 10
	Move_Data motionDatBuf[MOTION_DAT_BUF_SIZE];
	int motionDatIndex;
	bool bFirstReceive;

public:
     //CLASS VH_RENDER
	Move_Data M_DAT;

	MyBone *m_bone;
	//////////////////////
  //SLCamera
	SLCamera slcamera;
	bool FlagFace;
//SLCamera
/////////////////////////////////////////////
	bool OnTimer2();
	//10/21
	void BallAngle();		//ボールと頭の角度計算
	void CheckEyeAngle();	//目の回転角度を制限
	void SelectAngle();		//目でボールを追うか、ユーザの視線を追うか選択
	void ChangeFace();		//顔の選択
	//~10/21
/////////////////////////////////////////////

  //~SLCamera
	
  //MPI
	int press_key;
	int myrank;
	//MPI031021
	int diffMotionDatFrm;

	//SPIDAR-H
	DVDeviceManager devMan;
	HISpidar4quad h_spidar[2];

	float radius;					//	アバターの半径
	float hand_radius;	
    	
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
	float cStep;				//


	int nowMode;
	CmdSocket cmdSocket;
	TGLView(int nowMode);
	GLdouble bEye[3], bTarget[3], bUp[3];

	int cpNum;					// PC名識別用

	// Check collision
	bool bHCollide;
    bool bVCollide;
   //SOUND
	void Play_wave(char* StrFile);

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
	void Ball_Control(int cur_frm);
	void Step();
	void SetParam();
	bool LoadSettings();

	void conn_motion(int init_type, double k_time);
    void ballColliding(int cur_frm);
    void handColliding();
	void init_ball();
	void init_paddle();
	void init_hand();

//DRAW
   void drawBody(int cur_frame);
   void drawjoint(int cur_frm, MyBone sk_bone);
//shadow
/////////////////////////
	void DrawFloor(void);							//shadow
	void DrawFloor2(void);							//shadow

//SPIDAR-H
/////////////////////////////////////
  	bool bMinForce;
	bool holdHand;
	  // ボールに触れている手
	int ballCount;
	// 状態遷移カウンタ
    void init_spidar();
	void reset_spidar();
    void update_spidar();
	Vec3f convertEuclidToPole(Vec3f euclid);
	Vec3f convertPoleToEuclid(Vec3f pole);
    void spidarCheck();
	void BallIsFree();
	void BallIsHalfHolding();
	void BallIsHolding();
	void BallIsFixed();
	
////////////////////////////////////

//MPI
	void apply_send(int cur_frm);
    void apply_receive();

    std::ofstream *centerGFile;
	int printFileNameTime;
	char printFileName[16];

	int printHz;
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

