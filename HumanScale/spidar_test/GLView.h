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
// ����m�FSPIDAR�p ���[�^�̎��t�����W
#define PX	0.3125f			//	x�����̕ӂ̒���/2 [m]
#define PY	0.23f			//	y�����̕ӂ̒���/2 [m]
#define PZ	0.265f			//	z�����̕ӂ̒���/2 [m]
#define MAXON	0.3776f
#define LPP		2.924062107079e-5f
const float K = 10;	// �������܂���
#define SpidarUpdateRate	2
#define BallSpeedThreshold	2 * convert_inch	// ���������܂���
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
	
	//	�A�o�^�[�̑���
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	bool bFalling;				//	�A�o�^�[�������t���O
	float fallingTime;			//  �A�o�^�[��������
	
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

	float radius;					//	�A�o�^�[�̔��a
	float hand_radius;	
    	
	Affinef afBody;				//	�A�o�^�[�̈ʒu
	Affinef afInitialBody;		//  �A�o�^�[�̏����ʒu
	Vec3f VecMyself[3];			//  �������g�̈ʒu
	
	TGeoNodes allNodes[3];	//	���ׂẴm�[�h
	Vec3f clearColor[3];	//	��, ��̐F
	
	bool bSender;				//	���M���Ȃ�� true
	int viewMode;				//  ���_�̃��[�h (0:�A�p�^ 1:�ォ�王�_ 2:���Վ��_)
	int viewTurn;				//  ���_�̉�]�p
	float topHeight;			//  �ォ�王�_�̍���
	float topShift;				//  �ォ�王�_�̒��S����̂���
	float birdsHeight;			//  ���Վ��_�̍���
	float birdsRadius;			//  ���Վ��_�̊p�x
	float birdsShift;			//  ���Վ��_�̒��S����̂���
	float cRadius;				//  ���a
	float cStep;				//

	int nowMode;
//	CmdSocket cmdSocket;
	TGLView(int nowMode);
	GLdouble bEye[3], bTarget[3], bUp[3];

	int cpNum;					// PC�����ʗp

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
	  // �{�[���ɐG��Ă����
	int ballCount;
	// ��ԑJ�ڃJ�E���^
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

// ishikawa �{�[���p�x
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

