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

	float statusOfUser, lastStatusOfUser;   //  ���[�U�̏�Ԕ���p�ϐ�
	float tableAngle, lastTableAngle;       //  �^�[���e�[�u���̊p�x�f�[�^

	//	�Փ˔���
	float radius;					//	�A�o�^�[�̔��a
	TClosestPointDetectors cpds;	//	�ŋߖT�_�̌��o��
	TReflectNormalCalculator rnc;	//	���˖ʂ̖@���̌v�Z�@
	TPenaltyCanceler penCancel;		//	�y�i���e�B�����̌v�Z

	//	�A�o�^�[�̑���
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	Vec3f vel;					//	�A�o�^�[�̑��x
	Vec3f dvel;					//	�A�o�^�[�̑��x�̕ω�
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;
public:
	Affinef afBody;				//	�A�o�^�[�̈ʒu
	TGeoNodes allNodes;			//	���ׂẴm�[�h
	Vec3f clearColor;			//	��̐F
	bool bSender;				//	���M���Ȃ�� true
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

