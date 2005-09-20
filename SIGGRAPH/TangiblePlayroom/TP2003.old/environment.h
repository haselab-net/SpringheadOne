#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Base/VRBase.h> //Vec3f�̂��߂����ɕK�v
#include "GameView.h" 

//SPIDAR�^�C�v�𐮐���`
#define Env_NO_SPIDAR		0
#define Env_ISA_BIG_SPIDAR	1
#define Env_USB_SPIDAR_G	2

//����ȂƂ���ɏ����Ĉ�������SPRITE�̖���
#define MAX_SPRITES 9
#define MAX_LANGUAGES 5

#define MOUSE_SAMPLE 100 //�}�E�X�̕��σt���[����
// �`�悪�x�����̂���
//#define PH_GRAPHIC_LOW_DETAIL

using namespace LVRM;
/*
 environment.h
 �������ݒ�N���X
*/

//���������`����N���X
class Environment {
public:
	Environment(); //�R���X�g���N�^
	const char * filename; //���ݒ�t�@�C���̖��O
	bool Load(); //�t�@�C�����[�h

	//�ȉ��ݒ�t�@�C���̋L�q���Ɠ����B
	int idClient; //�N���C�A���g���`����ID�A�[���̓T�[�o
	char nameServer[255];	//�ڑ�����T�[�o��IP�������͖��O
	bool bUseSound;	//�T�E���h���g�p����ꍇ"UseSound"���w�肷��
	bool bUseGraphics;	//�O���t�B�b�N�X���g�p����ꍇ"UseGraphics"���w�肷��
	int iSpidar_type; //�g�p������SPIDAR�̃^�C�v�ڍׂ͏��define���Ă܂�
	//�ȉ��A�ߋ��̃\�[�X�Ƃ̌݊��̂��߂ɑ��݁B
	bool bClient; //�N���C�A���g�Ȃ�true,idClient����ݒ�B
	bool bSpidarExist; //SPIDAR�����݂���Ȃ�true
	float	px,py,pz;	//SPIDAR�����t�����Ă���ӂ̒���/2
	int	idExperiment;	//����ID�A�[���̓Q�[�����[�h

	bool bFullscreen;  //�t���X�N���[���`��
	bool bRoll90;      //�v���W�F�N�^����90�x��]���[�h
	bool bHaptics;     //�͊o���[�h

	//�{���͂����ɂ����Ăق����Ȃ��L�^�p�f�[�^�̈�
	Vec3f dForce; //�L�^���ԓ���SPIDAR�o�͂̑��a
	float dRatio;

	GameView	*view; //���ł��ǂ��ł��A�N�Z�X�ł���悤�ɂ��邽�߂�����

	int iStaticFrame; //Grasp��Ԃ𔻒f���邽�߂́ASPIDAR���͒l�������t���[���̐�
	int iGraspFrame;  //�������c����Ԃ̃t���[����
	Vec3f	vSpidar;  //SPIDAR�����W
	Vec3f	vSpidarLast;  //SPIDAR���W,�ЂƂ܂�
	Vec3f   vSpidarForce; //SPIDAR�w�ߒl
	Vec3f   vSpidarForceLast; //SPIDAR�w�ߒl,�ЂƂ܂�
	Vec3f	viewscale; //�f����SPIDAR�̍��W���킹


	//���S�΍�
	bool	bEmergency; //���u���[�L����

	//������Ή�
	int		iLanguage;  //����ID{���{��,�p��,...}

	//�}�E�X
	int mousex[MOUSE_SAMPLE];
	int mousey[MOUSE_SAMPLE];
	int mousez[MOUSE_SAMPLE];
	Vec3f v3DMouse;  //���z�O�����}�E�X

	bool bTallNone; //�����������[�h

};


extern Environment env;
#endif
