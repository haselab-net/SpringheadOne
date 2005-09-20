#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"
#include "HIS/HIStepWalk.h"
#include "HIS/HITurnTable.h"
#include "../Share/WalkThroughTools.h"
#include "EasyDS.h"
#include "EasyDS.h"
#include "LabelingCamera.h"
#include "Stereo.h"

//#define USE_SOUND
#define PC_CLUSTER_NUM 24

using namespace std;
using namespace LVRM;

class TGLView: public RDGlutView{
	int mouseX, mouseY;
//	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	float lastTableAngle;
	
	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;

	//	�Փ˔���
	float radius;					//	�A�o�^�[�̔��a
	float hand_radius;				//	��̔��a�@�ǉ��F�ԉH
	TClosestPointDetectors cpds;	//	�ŋߖT�_�̌��o��
	TClosestPointDetectors LHand_cpds;	//	����ŋߖT�_�̌��o��@�ǉ��F�ԉH
	TClosestPointDetectors RHand_cpds;	//	�E��ŋߖT�_�̌��o��@�ǉ��F�ԉH
	TReflectNormalCalculator rnc;	//	���˖ʂ̖@���̌v�Z�@
	TPenaltyCanceler penCancel;		//	�y�i���e�B�����̌v�Z

	//	�A�o�^�[�̑���
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	Vec3f vel;					//	�A�o�^�[�̑��x
	Vec3f dvel;					//	�A�o�^�[�̑��x�̕ω�
	bool bFalling;				//	�A�o�^�[�������t���O
	float fallingTime;			//  �A�o�^�[��������
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;

	// DirectSound�N���X
#ifdef USE_SOUND
	CEasyDS *easyds;
#endif

public:
	Affinef afLHand;			//  �A�o�^�[�̍���̈ʒu�@�ǉ��F�ԉH
	Affinef afRHand;			//  �A�o�^�[�̉E��̈ʒu�@�ǉ��F�ԉH

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
	bool showInnerData;			//  �����f�[�^(�A�p�^�ʒu�E���̓Z���T���́E�d�S�Ȃ�)��\�����邩
	int cSides;				//  �p�̐�
	double PI2;				//  �~���p
	float cStep;				//
	Vec3f VecCircle[32];
	Vec3f VecSquare[4];
	Vec3f VecSensor[4];
	Vec3f VecStartPosition[4];

	Vec2f local_eyedir;         // sender�����M������������
	int cpNum;					// PC�����ʗp
	int SyncBuffer[PC_CLUSTER_NUM];
	int ReadyNum;

	int nowMode;
	CmdSocket cmdSocket;
	TGLView(int nowMode);
	GLdouble bEye[3], bTarget[3], bUp[3];
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

	void OutPutSIGGRAPHData();
	void CloseOutputFile();


	std::ofstream *centerGFile;
	int printFileNameTime;
	char printFileName[16];

	std::ofstream *turnDataFile;
	int printTurnDataTime;
	char printTurnData[16];

	std::ofstream *dataFileSIGGRAPH;
	int fileNameSIGGRAPHTime;
	char fileSIGGRAPHTime[16];

	char* folderName;
	char folderNameBack[128];

	void PrintHz(std::ostream& os) const;
	int stepHz;
	std::ofstream *printStepHz;

protected:
	float dwLastTime;
	float dwTime;
	float logStartTime;

	float jumpPAve;  //�W�����v����͑���p
	float jumpRate;
	int jumpFlag;
	int numSensor;

	bool getSIGGRAPHData;
	int logStep;

	SLCamera slcamera;//�X�e���I���x�����O�J����

	StereoV stVision;//�X�e���I���p
	Affinef afC;

	Vec3f labelP[2];

};

#endif

