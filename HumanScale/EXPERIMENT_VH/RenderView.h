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
//#include "Camera/LabelingCamera.h"
//~SLCamera
///////////////////////
#include "direxec.h"
////////////////////////

//shadow
#include "mySoftShadowMapVH.h"

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
#define SpidarUpdateRate	2
const float convert_scale = 39.87; //100/2.508
#define BallSpeedThreshold	2 * convert_scale	// ���������܂���
const float K = 10;	// �������܂���
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

//CLASS VH_RENDER
	MyBone *m_bone;

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
	
	TGeoNodes allNodes[3];			//	���ׂẴm�[�h
	Vec3f clearColor[3];			//	��̐F
	
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

	// Check collision
	bool bHCollide;
    bool bVCollide;

	GLfloat deltaRot;
    GLfloat nRange; 
    GLfloat nHeight; //45.0
    GLfloat delta_Pad;
    GLfloat GnHeight; //

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
	bool OnTimer2();
	void OnIdle();

	void Ball_Control(int);
	void Step();
	void SetParam();
	bool LoadSettings();

	void conn_motion(int);
	void transition_motion(int);
	void DrawTextXY(double x,double y,double z,double scale,char *s);
    void ballColliding(int);
    void handColliding();
	bool bboxColliding();
	void init_ball();
	void init_paddle();
	void init_hand();
	//2004.10
	void init_db();

//DRAW
   void drawBody(int cur_frame);
   void drawjoint(int cur_frm, MyBone sk_bone);
   void drawHand(int cur_frame); //2004.10 experiment
   void drawBall();
   void drawBody_other(int idx, int cur_frame);

 //shadow�N���X�ɕK�v�ȃ����o�ϐ����֐�
//////////////////////////////////////
    //�ϐ�
    CMySoftShadowMap *Shadow;
	float eyeView[4];
	float s_delta;
	int winWidth, winHeight;
	//�֐�
   	void renderScene(void);
	void Processing_shadow(Affinef);

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
///For Experiment 
	void spidar_save();
	void shot_control();
	void text_clear();
//////////////////////////////////////////
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

