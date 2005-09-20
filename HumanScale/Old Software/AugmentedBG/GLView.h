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
#include "LabelingCamera.h"
#include "Stereo.h"

using namespace LVRM;

class TGLView: public RDGlutView{
	float nMouseX, nMouseY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	float lastTableAngle;
	int first[2], second[2];	//  �}�E�X���W�̗���
	Vec3f mouseVector;			//  �}�E�X�̈ړ��x�N�g��
	bool bBox;					//  �o�E���f�B���O�{�b�N�X�̕\���^��\��
	bool avatarHit;				//  �A�o�^�Ƀq�b�g���Ă��邩�ǂ���
	bool objectHit;				//  �I�u�W�F�N�g�Ƀq�b�g���Ă��邩�ǂ���

	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;
	float birdX, birdZ;

	//	�Փ˔���
	float radius;						//	�A�o�^�[�̔��a
	float hand_radius;					//	��̔��a�@�ǉ��F�ԉH
	TClosestPointDetectors cpds;		//	�ŋߖT�_�̌��o��
	TClosestPointDetectors LHand_cpds;	//	����ŋߖT�_�̌��o��@�ǉ��F�ԉH
	TClosestPointDetectors RHand_cpds;	//	�E��ŋߖT�_�̌��o��@�ǉ��F�ԉH
	TReflectNormalCalculator rnc;		//	���˖ʂ̖@���̌v�Z�@
	TPenaltyCanceler penCancel;			//	�y�i���e�B�����̌v�Z

	//	�A�o�^�[�̑���
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	Vec3f vel;					//	�A�o�^�[�̑��x
	Vec3f dvel;					//	�A�o�^�[�̑��x�̕ω�
	bool bFalling;				//	�A�o�^�[�������t���O
	float fallingTime;			//  �A�o�^�[��������
	Affinef bird;				//  ���ՃJ��������p
	Affinef quarter;			//  �N�H�[�^�[�r���[�H�H�J��������p
	Vec4f planeNormal;			//  ���ʂ̖@���x�N�g��
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;
	Vec4f worldNear, worldFar;
	Vec4f collisionPoint;
	
	bool bHistorySave;						//  �O�ՋL�^�p�t���O
	bool bHistoryDraw;						//  �O�Օ\���t���O
	bool bHistoryToFile;					//  �O�Ճt�@�C�����������t���O//
	typedef std::vector<Vec3f> History;		//  �O�Օۑ��p
	History historyData;
	typedef std::vector<DWORD> Times;		//  ���ԕۑ��p
	Times currentTime;
	bool bTest;								//  �e�X�g�p�Ö��ݒ�

public:
	Vec3f avatarMoveVector;			//  �A�o�^�[�̈ړ��x�N�g��
	Vec3f objectMoveVector;			//  ���̂̈ړ��x�N�g��
	bool bZoom;						//  �Y�[�����Ȃ�true
	Affinef beforeRotateAfBody;		//  �}�E�X�ŉ�]����O�̃A�o�^�[�̎p��
	bool nowRotateBirdCam;			//  ���ՃJ�����̉�]���Ȃ�true
	float birdCamRotateAngle;		//  ���ՃJ�����̉�]�p�x
	float tmpElevation[2];
	float elevationHeight;			//  ���Վ��_�̍��x
	float elevationDistance;		//  ���Վ��_�̋���
	DVOrientationBase* elevationSensor;
	float startElevationAngle;
	float viewElevation;
	Affinef afLHand;				//  �A�o�^�[�̍���̈ʒu�@�ǉ��F�ԉH
	Affinef afRHand;				//  �A�o�^�[�̉E��̈ʒu�@�ǉ��F�ԉH
	int tmpMouseY[2];				//  �}�E�X�ʒu�̗���
	int mouseX, mouseY;				//  �X�N���[����̃}�E�X�̈ʒu
	int startMouseY;				//  �E�N���b�N�����Ƃ��̃}�E�X�̈ʒu
	Affinef afProjection;
	Affinef afCam;
	Affinef afQuarter;				//  �N�I�[�^�[�r���[�H�p�̃J����
	Affinef afTmpB;
	bool nowLeftBottunOn;			//  ���݃}�E�X�̍��{�^����on�Ȃ�true
	bool nowRightBottunOn;			//  ���݃}�E�X�̉E�{�^����on�Ȃ�true
	int windowWidth, windowHeight;	//  �E�B���h�E�̏c�Ɖ��̃h�b�g��
	Affinef afBody;					//	�A�o�^�[�̈ʒu
	Affinef afInitialBody;			//  �A�o�^�[�̏����ʒu
	Affinef afBird;					//  ���Վ��_
	Affinef afBirdRot;				//  ���Վ��_�J�����̉�]����p
	Vec3f VecMyself[3];				//  �������g�̈ʒu
	Vec3f VecTest[4];				//  �e�X�g�p�̎l�p�`
	TGeoNodes allNodes;				//	���ׂẴm�[�h
	Vec3f clearColor;				//	��̐F
	bool bSender;					//	���M���Ȃ�� true
	bool birdsView;					//  ���Վ��_�Ȃ��true
	bool quarterView;				//  �N�I�[�^�[�r���[�Ȃ��true
	bool quarterViewM;
	int viewMode;					//  ���_�̃��[�h (0:�A�p�^ 1:�ォ�王�_ 2:���Վ��_)
	int viewTurn;					//  ���_�̉�]�p
	float topHeight;				//  �ォ�王�_�̍���
	float topShift;					//  �ォ�王�_�̒��S����̂���
	float birdsHeight;				//  ���Վ��_�̍���
	float birdsRadius;				//  ���Վ��_�̊p�x
	float birdsShift;				//  ���Վ��_�̒��S����̂���
	float cRadius;					//  ���a
	bool showInnerData;				//  �����f�[�^(�A�p�^�ʒu�E���̓Z���T���́E�d�S�Ȃ�)��\�����邩
	int cSides;						//  �p�̐�
	double PI2;						//  �~���p
	float cStep;					//
	Vec3f VecCircle[32];
	Vec3f VecSquare[4];
	Vec3f VecSensor[4];
	Vec3f VecStartPosition[4];
	bool nowElevation;

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
	void Move();
	void CamRotate();
	void InitElevation();
	void NowElevation();
	void InitElevationM();
	void NowElevationM();
	void InverseTransform();
protected:
	float dwLastTime;
	float dwTime;

//	float jumpPAve;  //�W�����v����͑���p
	float jumpRate;
	int jumpFlag;
	int numSensor;

	bool getSIGGRAPHData;

	SLCamera slcamera;//�X�e���I���x�����O�J����

	StereoV stVision;//�X�e���I���p
	Affinef afC;

	Vec3f labelP[2];

};

#endif

