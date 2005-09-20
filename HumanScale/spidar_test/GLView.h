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
struct Move_Data
{
   //animation info
	int Anim_Set[10];
    //joint motion per one frame
    float Cur_Bone[60]; //channel
	float End_Effector[6];
//Add	
	float Ball_Pos[3];
	float Hand_Pos[6];
	int Temp_Rot[9];
	//ADD take
	float RotBall[3];

};

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

	//MPI031021
	#define MOTION_DAT_BUF_SIZE 10
	Move_Data motionDatBuf[MOTION_DAT_BUF_SIZE];
	int motionDatIndex;
	bool bFirstReceive;

public:

	//MPI
	Move_Data M_DAT;
/////MPI  /////////////////////////////
	int press_key;
	int myrank;
	//MPI031021
	
	int diffMotionDatFrm;

	float radius;					//	アバターの半径
	float hand_radius;	
    	
	Affinef afBody;				//	アバターの位置
	Affinef afInitialBody;		//  アバターの初期位置
	Vec3f VecMyself[3];			//  自分自身の位置
	
	TGeoNodes allNodes[3];	//	すべてのノード
	Vec3f clearColor[3];	//	空, 手の色
	
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

	//Camera
	Vec3f vCamera;
	// Check collision
	bool bHCollide;
    bool bVCollide;
   //SOUND///////////////////////
	void Play_wave(char* StrFile);
   ///////////////////////////////////
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnSpecialKey(int key, int x, int y); 
////////////////////////////////////////////
	void OnOpen();
	bool OnTimer();
	void OnIdle();

	void Ball_Control(int cur_frm);
	void Step();
	void SetParam();
	bool LoadSettings();

    void ballColliding(int cur_frm);
    void handColliding();

	void init_ball();
	void init_hand();

//shadow
/////////////////////////
    void DrawShadow(void);   //jeong
	void DrawFloor(void);							//shadow

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

