#ifndef TGLView_H
#define TGLView_H

#include <vector>
#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include "communication.h"
#include <HIS/HIStepWalk.h>
#include <HIS/HITurnTable.h>
#include "../Share/WalkThroughTools.h"

using namespace LVRM;

class TGLView: public RDGlutView{
	float nMouseX, nMouseY;
	Vec4f worldMouse;
	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	float lastTableAngle;
	Vec3f mouseVector;			//  �}�E�X�̈ړ��x�N�g��
	bool bBox;					//  �o�E���f�B���O�{�b�N�X�̕\���^��\��
	bool avatarHit;				//  �A�o�^�Ƀq�b�g���Ă��邩�ǂ���
	bool avatarDMove;			//  �A�o�^���u�Ԉړ����邩�ǂ���
	bool objectHit;				//  �I�u�W�F�N�g�Ƀq�b�g���Ă��邩�ǂ���
	bool objectDMove;			//  �I�u�W�F�N�g���u�Ԉړ����邩�ǂ���

	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;
	float birdsHight;
	float birdX, birdZ;

	//	�Փ˔���
	float radius;					//	�A�o�^�[�̔��a
	TClosestPointDetectors cpds;	//	�ŋߖT�_�̌��o��
	TReflectNormalCalculator rnc;	//	���˖ʂ̖@���̌v�Z�@
	TPenaltyCanceler penCancel;		//	�y�i���e�B�����̌v�Z

	//	�A�o�^�[�̑���
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	Vec3f vel;					//	�A�o�^�[�̑��x
	Vec3f dvel;					//	�A�o�^�[�̑��x�̕ω�
	Vec4f planeNormal;			//  ���ʂ̖@���x�N�g��
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;
	Vec4f worldNear, worldFar;
	Vec4f collisionPoint;
	bool bHistorySave;				//  �O�ՋL�^�p�t���O
	bool bHistoryDraw;				//  �O�Օ\���t���O
	bool bHistoryToFile;			//  �O�Ճt�@�C�����������t���O//
	typedef std::vector<Vec3f> History;		//  �O�Օۑ��p
	History historyData;
	typedef std::vector<DWORD> Times;		//  ���ԕۑ��p
	Times currentTime;
public:
	float mouseX, mouseY;
	Affinef afProjection;
	Affinef afCam;
	Affinef afTmpB;
	TGeoNode* hitNode;				//  �����蔻��Ɉ������������m�[�h�̃|�C���^�i�[�p
	int windowWidth, windowHeight;	//  �E�B���h�E�̏c�Ɖ��̃h�b�g��
	Affinef afBody;					//	�A�o�^�[�̈ʒu
	Affinef afBird;					//  ���Վ��_
	Vec3f VecMyself[3];				//  �������g�̈ʒu
	Vec3f VecTest[4];				//  �e�X�g�p�̎l�p�`
	TGeoNodes allNodes;				//	���ׂẴm�[�h
	Vec3f clearColor;				//	��̐F
	bool bSender;					//	���M���Ȃ�� true
	bool birdsView;					//  ���Վ��_�Ȃ��true
	int nowMode;
	float screenNear;				//  �����_�����O�{�����[���̑O�ʈʒu
	float screenFar;				//  �����_�����O�{�����[���̔w�ʈʒu
	int hitID;						//  ���������{�b�N�X��ID
	bool firstDraw;					//  ��x�ڂ̕`�悩�ǂ����̔���p
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
	void LightSettings();
	void InitCollisionDetector();
	void Collision();
	void DataSave();
protected:
	float dwLastTime;
	float dwTime;
};

#endif

