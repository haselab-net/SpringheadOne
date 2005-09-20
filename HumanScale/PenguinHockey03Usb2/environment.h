#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Base/VRBase.h> //Vec3f�̂��߂����ɕK�v

//SPIDAR�^�C�v�𐮐���`
#define Env_NO_SPIDAR		0
#define Env_ISA_BIG_SPIDAR	1
#define Env_USB_SPIDAR_G	2

using namespace LVRM;
/*
 environment.h
 �������ݒ�N���X
*/

//���������`����N���X
class Environment {
public:
	const char * filename; //���ݒ�t�@�C���̖��O
	bool Load();

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
	bool bHaptics;     //�͊o���[�h

	//�{���͂����ɂ����Ăق����Ȃ��L�^�p�f�[�^�̈�
	Vec3f dForce; //�L�^���ԓ���SPIDAR�o�͂̑��a
	float dRatio;

};


extern Environment env;
#endif
