#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"
#include "HIS/HIStepWalk.h"		//#include <HIS/HIStepWalk.h>
#include "HIS/HITurnTable.h"	//#include <HIS/HITurnTable.h>

#include "Seesaw.h"		// �V�[�\�[�̂��߂Ɏg��
#include "MoveFloor.h"  // �������̂��߂Ɏg��
#include "JumpZone.h"	// �W�����v�]�[���̂��߂Ɏg��
#include "UdpRecv/udprecv.h" // ���[�U�[�|�C���^�̎�M�Ɏg��
#include "TGeoNodePointer.h" // ���[�U�[�|�C���^�m�[�h

#include "../Share/WalkThroughTools.h"
#include "cylinder_lib/BacicIO2.h"

using namespace LVRM;
using namespace vroadrunner;

#define TIMER_SIZE 3 // timer�\���̌���
#define VASH_SIZE 4 // vash�\���̌���

#define SOUND_ATTACK 0
#define SOUND_COIN 1
#define SOUND_JUMP 2
#define SOUND_WATER 3
#ifdef USE_SOUND
#include "EasyDS/EasyDS.h"
#endif

class TGLView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY; //??? �g���Ă��Ȃ�
	Button mouseButton;
	double setting[6]; //?? ���p�ړI���悭�킩��Ȃ� -> �v���W�F�N�^�[�̐ݒu�ʒu�Ɋւ���ݒ�l�ł͂Ȃ����H
	Affinef R; //?? �悭�킩��Ȃ�
	double fov_v; //? glutPerspective()�@��y��������(�i�K��)����p�B

	float lastTableAngle; //�@�^�[���e�[�u���̊p�x����
	float changedAngle;   //  �^�[���e�[�u���̊p�x�ω���
	int first[2], second[2];	//  �}�E�X���W�̗���
	Vec3f mouseVector;			//  �}�E�X�̈ړ��x�N�g��
	bool hitFlag;				//  �����蔻��p
	bool bBox;					//  �o�E���f�B���O�{�b�N�X�̕\���^��\�� //?? �o�E���f�B���O�{�b�N�X�Ƃ́H

	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;
	float birdsHight;

#ifdef USE_SOUND
	CEasyDS *easyds;
#endif

	//	�Փ˔���
public:
	float radius;					//	�A�o�^�[�̔��a
protected:
	TClosestPointDetectors cpds;	//	�ŋߖT�_�̌��o��

#ifdef _SPIDAR_TEST
	float hand_radius;//	��̔��a
#endif //_SPIDAR_TEST

	TReflectNormalCalculator rnc;	//	���˖ʂ̖@���̌v�Z�@
	TPenaltyCanceler penCancel;		//	�y�i���e�B�����̌v�Z

	bool bGravity;		//	�d�͂������邩�ǂ����i�������Ŏg�p�j

	//	�A�o�^�[�̑���
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	Vec3f vel;					//	�A�o�^�[�̑��x
	Vec3f dvel;					//	�A�o�^�[�̑��x�̕ω�
	Vec4f avatarNormal;			//  �A�o�^�[�̖@���x�N�g��
	Affinef afCam;				//  �J�����̈ʒu
	bool bLastIsJump;			//  �O���Step��isJump��ۑ�
	bool bFallingPond;			//  �r�ɗ����Ă���Ƃ���true

	bool bDraw; //?? �`��v�������邩�ǂ���
	bool bDrawNormal;
	bool bDrawCollision;
	Vec4f worldNear, worldFar;
	Vec4f collisionPoint;


#ifdef _KATSU_TEST_APP
	//	�V�[�\�[
	CSeesaw aSeesaw[8];
#endif //_KATSU_TEST_APP

//#ifdef _OKADA_TEST_APP
	//�@������
	CMoveFloor aMoveFloor, bMoveFloor, cMoveFloor;
//#endif //_OKADA_TEST_APP

//#ifdef _OKADA_TEST_APP
	//  �W�����v�]�[��
	CJumpZone aJumpZone, bJumpZone, cJumpZone, dJumpZone, eJumpZone, fJumpZone, gJumpZone;
//#endif //_OKADA_TEST_APP

	//cylinder
	PIO pio;

	// ���[�U�[�|�C���^
	float laserX,laserY; //�|�C���^�̍��W(��ʂ�X,Y)
	bool laserCaptured; //�|�C���^���_�����Ă��邩
	bool lastLaserCaptured; // �O��̍X�V���Ƀ|�C���^���_�����Ă�����
	CUdpRecv *udpRecv;
	Affinef afLaser; // �J�����ʒu���猩���|�C���^�̍��W afPoint = afCam * afLaser;�݂����Ɏg���B
	Vec3f vecLaser; // ���[���h���W�Ō����A�J�����ʒu����|�C���^�ʒu�ւ̃x�N�g��
	TGeoNodePointer *pLaserNode;

public:
	void PlaySound( int SoundNum );
	void FallPond(); // �r�ɗ������Ƃ��ɃA�o�^�\��߂�
	GLfloat bodyHeight;
	int GameTimerSize;
	char GameCommentStr[126];
	void DrawVashVal();
	bool LaserAttack();
	~TGLView();
	bool isGameStart;
	void GameStart();
	bool isGameOver;
	void GameOver();
	void AddTimerVal(int val = 1);
	void GetTimerChar();
	void GetVashChar();
	void AddVashVal(int val);

	int vash_val;
	char vash_char[VASH_SIZE+1]; //3��

	int timer_val;
	char timer_char[TIMER_SIZE+1]; //4��
	DWORD timer_start;

	bool isSpotLight;
	void SpotLightSetting();
	void LightSetting();
	TGeoNode* hitNode;			//  �����蔻��Ɉ������������m�[�h�̃|�C���^�i�[�p
	int windowWidth, windowHeight;	//  �E�B���h�E�̏c�Ɖ��̃h�b�g��
	
#ifdef _SPIDAR_TEST
	Affinef afLHand;			//  �A�o�^�[�̍���̈ʒu�@�ǉ��F�ԉH
	Affinef afRHand;			//  �A�o�^�[�̉E��̈ʒu�@�ǉ��F�ԉH
#endif //_SPIDAR_TEST


	Affinef afBody;				//	�A�o�^�[�̈ʒu
	Affinef afBird;				//  ���Վ��_
	Vec3f VecMyself[3];			//  �������g�̈ʒu
#ifdef _DEBUG
	Vec3f VecTest[4];			//  �e�X�g�p�̎l�p�`
#endif //_DEBUG
	TGeoNodes allNodes;			//	���ׂẴm�[�h
	Vec3f clearColor;			//	��̐F
	bool bSender;				//	���M���Ȃ�� true
	bool birdsView;				//  ���Վ��_�Ȃ��true
	int nowMode;
	float screenNear;			//  �����_�����O�{�����[���̑O�ʈʒu
	float screenFar;			//  �����_�����O�{�����[���̔w�ʈʒu
	CmdSocket cmdSocket;
	TGLView(int nowMode);
	GLdouble bEye[3], bTarget[3], bUp[3];
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	//void OnJoystick(unsigned int button, int x, int y, int z);
	bool OnTimer();
	void OnIdle();
	void Step();
	//void SetParam();
	//bool LoadSettings();
	void InitCollisionDetector();
	//bool CollisionMouse();
	bool showInnerData;
	Vec3f VecSensor[4];
	Vec3f VecStartPosition[4];
	
	TGeoNodePointer* GetLaserNode(){ return pLaserNode; }

protected:
	float dwLastTime;
	float dwTime;
	float playerAngle; //�v���[���[�̐��ʕ�����Ǐ]
};

#endif
