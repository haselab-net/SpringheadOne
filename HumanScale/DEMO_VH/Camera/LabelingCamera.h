////���x�����O�J�����p<-���virtualDevice�ɂ��������ǂ��̂ł�

#ifndef __LABELING_CAMERA_H__
#define __LABELING_CAMERA_H__

#include <Base/Affine.h>
#include <WinBasis/WBWin.h>
#include "CySLC.h"
#include "SLCDevelop.h"
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace LVRM;

#define PI1 3.14159265358979
/*
typedef struct
{
	double	dX;			//���̂�X���W
	double	dY;			//���̂�Y���W
	double	dZ;			//���̂�Z���W
	int numLabel;		//���x�����ʔԍ�(0�ȏ�)
	int nStatus;			//�X�e�[�^�X 0:���� 1�ȏ�:�L��
} LabelData;
*/

class SLCamera{
protected:
	HANDLE lCamera;
	int labelNum;           //���x�����O�J�����Ŏ擾���郉�x���̐�
	enum {labelNumMax = 64};//�ꉞ64�ɂ��Ă��邪���Ȃ����Ă��\��Ȃ��̂ł́D
	int lastLabelNum;
	Vec2f eyeDirection,eyeDir,direction;
	Vec2f leftEye2D, rightEye2D;
	Vec3f leftEye, rightEye, topEye;
	Vec3f centerPoint, rightEyeVec, thirdEyeVec;
	enum {LABEL_NUM = 3};
	Vec3f trackingEye[LABEL_NUM],dtopEye[LABEL_NUM],dLeftEye[LABEL_NUM],dRightEye[LABEL_NUM];
	float alpha, tmp; //eyeDirection�̕ω��ɑ΂���t�B���^�̌���
	Vec2f EyeDirection(); //�����������v�Z
	SLCOBJECT labelPos[labelNumMax]; //���x���̏����i�[����

	Vec3f eyeDirection3D;
	int AngleSide;			//X-Z���ʂ̊p�x
	int AngleVert;			//Y-Z���ʂ̊p�x
	int PreLabelNum;		//���x�����O�̐�
	Vec3f eyeDirection2;//��������
	int Angle[2][2];	//[0:AngleSide,1:AngleVert][0:��O�A1:����]
//	int *PreEyeAngle;

	Affined IdealPos;
	Affined CallibAffin;
	Vec4d NowEyePos[3];
	bool Calib;


public:
	//Vec3f eyeDirection3D;
	SLCamera();
	~SLCamera();
	bool InitSLC(); //new
	//bool InitSLC(int* PreMainEyeAngle);
	void CloseSLCamera();
	bool Step();			//�ʒu�擾�A���������A�p�x�v�Z
	
	int GetLabelPos(Vec2f* labelPosition);//�����ɑ΂��ă��x���̍��W���i�[����܂��D�Ԃ�l�̓T�C�Y(���x���̐�)�ł��D
	int GetLabelPos(Vec3f* labelPosition);
	Vec2f GetEyeDirection(){ return eyeDirection; };//����������Ԃ��܂��D
	float GetEyeAngle();

	Vec3f GetEyeDirection2();	//�����������v�Zosada��
	void ShowData();		//�e�p�x��\��
	int AdjustAngleSide();	//���������̊p�x�𒲐�
	int AdjustAngleVert();	//���������̊p�x�𒲐�
	float GetEyeAngleSide();//���������̊p�x���擾
	float GetEyeAngleVert();//���������̊p�x���擾
	//10/21
	void CallibSLC();		//�J�����̃L�����u���[�V����

};

#endif