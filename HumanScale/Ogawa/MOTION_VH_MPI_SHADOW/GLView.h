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
///////////////////////
#include "direxec.h"
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
////////////////////////////////////////////
//shadow
//#include <ATIExtensions.h>
#include "mySoftShadowMapVH.h"
/////////////////////////////////////////////


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
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;
	
	//MPI031021
	#define MOTION_DAT_BUF_SIZE 10
	Move_Data motionDatBuf[MOTION_DAT_BUF_SIZE];
	int motionDatIndex;
	bool bFirstReceive;


public:

//CLASS VH_RENDER
	MyBone *m_bone;
	//VH Check collision
	bool bHCollide;
    bool bVCollide;

//MPI
	Move_Data M_DAT;
	int cpNum;					// PC名識別用
	int myrank;
	int press_key;
	int diffMotionDatFrm;

	void apply_send(int cur_frm);
    void apply_receive();
/////////////////////////////////////////
//VM Load view point
	float radius;					//	アバターの半径
	float hand_radius;	

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
	Affinef afBody;				//	アバターの位置
	Affinef afInitialBody;		//  アバターの初期位置
	GLdouble bEye[3], bTarget[3], bUp[3];
	Vec3f VecMyself[3];			//  自分自身の位置

	void SetParam();
	bool LoadSettings();
/////////////////////////////////////	
	TGeoNodes allNodes[3];			//	すべてのノード
	Vec3f clearColor[3];			//	空の色

	int nowMode;
	TGLView(int nowMode);

   //SOUND///////////////////////
	void Play_wave(char* StrFile);
   ///////////////////////////////////
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnSpecialKey(int key, int x, int y); 

	void OnOpen();
	bool OnTimer();
	void OnIdle();
   ///////////////////////////////////
	void Ball_Control(int);
	void Step();

	void conn_motion(int);
	void transition_motion(int);
	void DrawTextXY(double x,double y,double z,double scale,char *s);
    void ballColliding(int);
    void handColliding();
	bool bboxColliding();
	void init_ball();
	void init_paddle();
	void init_hand();

//DRAW
   void drawBody(int cur_frame);
   void drawjoint(int cur_frm, MyBone sk_bone);

//shadowクラスに必要なメンバ変数＆関数
//////////////////////////////////////
    //変数
    CMySoftShadowMap *Shadow;
	float eyeView[4];
	float s_delta;
	int winWidth, winHeight;
	//関数
    void DrawShadow(void);  //jeong
	void DrawFloor(void);	//shadow
	void renderScene(void);

//SPIDAR-H
/////////////////////////////////////
	DVDeviceManager devMan;
	HISpidar4quad h_spidar[2];

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
///For Experiment 
	void spidar_save();
	void shot_control();

	int stepHz;


protected:
	DWORD dwLastTime;
	DWORD dwTime;

	int statusOfUser;
	int lastStatusOfUser;

	Vec3f labelP[2];

};

#endif

