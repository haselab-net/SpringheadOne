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
#define LPP		2.924062107079e-5f
const float K = 10;	// 強くしました
#define SpidarUpdateRate	2
#define BallSpeedThreshold	2 * convert_inch	// 厳しくしました
/////////////////////////////////////////////
#define PC_NUM 24

//SLCamera
#define BallLogMax 20
#define AngleLogMax 33
//~SLCamera

class TGLView: public RDGlutView{
	int mouseX, mouseY;
//	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	
	//	アバターの属性
	Affinef afEye;				//	アバターから見た目の位置
	bool bFalling;				//	アバター落下中フラグ
	float fallingTime;			//  アバター落下時間
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;

	//SLCamear
	////////////////////////////////
	Vec3f BallDirection;
	int MainAngleSide;
	int MainAngleVert;
	int UserAngleSide;
	int UserAngleVert;
	int BallAngleSide;
	int BallAngleVert;
	Vec3f BallPosChange[BallLogMax];
	Vec3f BallPosLog2[BallLogMax];
	double BallPosChangeTotal;
	int RotateTurn; // 0：目だけ、1：頭の前の目、2：目の後の頭
	int PreMainAngleSide;
	int PreMainAngleVert;
	int FlagFace2; // 0：通常、1：キャッチ成功、2：キャッチ失敗
	bool DelayFlag;
	bool DelayFlag2;
	int PreAngleSide;
	int PreAngleVert;
	int AngleLog[2][AngleLogMax];
	int UserAngleLog[2][AngleLogMax];
	int BallAngleLog[2][AngleLogMax];
	int EyeAngleSide;
	int EyeAngleVert;
	bool EndCatchMiss;

	bool ConfirmAngle;
	/////////////////////////////////////

	//MPI031021
	#define MOTION_DAT_BUF_SIZE 10
	Move_Data motionDatBuf[MOTION_DAT_BUF_SIZE];
	int motionDatIndex;
	bool bFirstReceive;

public:

//CLASS VH_RENDER
	MyBone *m_bone;
/////////////////////////////////////////////
  //SLCamera
	SLCamera slcamera;
	bool FlagFace;
	void InitCamera(); //jeong
	bool OnTimer2();
	//10/21 -23
	void BallAngle();		//ボールと頭の角度計算
	void CheckEyeAngle();	//目の回転角度を制限
	void SelectAngle();		//目でボールを追うか、ユーザの視線を追うか選択
	void ChangeFace();		//顔の選択
	void SelectRotateTurn();

	void DrawPlayMode(int);
	void DrawCatchSucc();
	void DrawCatchMiss();
	void RotateEye();
	void RotateHead();
	void RotateEyeHeadDelay();
	void RotateEyeHead();
	//~10/23
//~SLCamera
/////////////////////////////////////////////
	
  //MPI
	Move_Data M_DAT;
/////MPI  /////////////////////////////
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
	
	//JEONG
	TGeoNodes allNodes[3];			//	すべてのノード
	Vec3f clearColor[3];			//	
	
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
//	CmdSocket cmdSocket;
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

	void conn_motion(int);
	void transition_motion(int);
    void ballColliding(int cur_frm);
    void handColliding();
	bool bboxColliding();
	void init_ball();
	void init_paddle();
	void init_hand();

//DRAW
   void drawBody(int cur_frame);
   void drawjoint(int cur_frm, MyBone sk_bone);
//shadow
/////////////////////////
    void DrawShadow(void);   //jeong
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

// ishikawa ボール角度
	Vec3f nowBallPole;
	
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

