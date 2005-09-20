#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"
//#include "LabelingCamera.h"
//#include "Stereo.h"
//SOUND
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

//SLCamera
#include "Camera\LabelingCamera.h"

using namespace std;
using namespace LVRM;
//SPIDAR-H
/////////////////////////////////////////////
#include <His/HISpidar4Quad.h>
#include <Device/DRUsb20Simple.h>
#define PX	1.2f	//	x�����̕ӂ̒���/2
#define PY	1.2f	//	y�����̕ӂ̒���/2
#define PZ	1.2f	//	z�����̕ӂ̒���/2
#define MAXON	0.3776f
#define LPP	2.924062107079e-5f
const float K = 10;
/////////////////////////////////////////////
#define PC_NUM 24

class TGLView: public RDGlutView{
	int mouseX, mouseY;
//	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	float lastTableAngle;
	
	//	�A�o�^�[�̑���
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	bool bFalling;				//	�A�o�^�[�������t���O
	float fallingTime;			//  �A�o�^�[��������
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;

	DVDeviceManager devMan;
	HISpidar4quad h_spidar[2];

public:

	//SLCamera
	SLCamera slcamera;

	//MPI
	int press_key;
	int myrank;

	float radius;					//	�A�o�^�[�̔��a
	float hand_radius;	
    	
	Affinef afBody;				//	�A�o�^�[�̈ʒu
	Affinef afInitialBody;		//  �A�o�^�[�̏����ʒu
	Vec3f VecMyself[3];			//  �������g�̈ʒu
	
	TGeoNodes allNodes;			//	���ׂẴm�[�h
	Vec3f clearColor;			//	��̐F
	
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
	CmdSocket cmdSocket;
	TGLView(int nowMode);
	GLdouble bEye[3], bTarget[3], bUp[3];

	int cpNum;					// PC�����ʗp
	int SyncBuf[PC_NUM];
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

	//SLCamera
	bool TGLView::OnTimer2();

	void OnIdle();
	void Ball_Control(int cur_frm);
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
//SPIDAR-H
/////////////////////////////////////
    void init_spidar();
	void reset_spidar();
    void update_spidar();
////////////////////////////////////
    std::ofstream *centerGFile;
	int printFileNameTime;
	char printFileName[16];

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

