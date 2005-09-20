#pragma once

#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>

//�J�u�g���V�R���g���[��
//�����F���̃��R�r�A��
/*
	(�Ҋ֐�Yaw, �Ҋ֐�Pitch, �G�֐�) = (q1, q2, q3)
	(�ڒ�, ����) = (l1, l2)
	J = {
		-c1(l1c2 + l2c(2-3))	 s1(l1s2 + l2s(2-3))	-l2s1s(2-3)
		-s1(l1c2 + l2c(2-3))	-c1(l1s2 + l2s(2-3))	 l2c1s(2-3)
		0							l1c2 + l2c(2-3)		-l2c(2-3)
	}
	det(J) = -l1l2(l1c2 + l2c(2-3))s3
	ci := cos(qi), si := sin(qi)
*/
enum {LEFT, RIGHT};
enum {FRONT, MIDDLE, BACK};
enum {YAW, PITCH, KNEE, HAND};
enum BeetleState{STOP, NEUTRAL, FORWARD, BACKWARD, LEFTTURN, RIGHTTURN};
	
class Beetle
{
public:
	class Leg
	{
		Spr::Vec3f	Error, ErrorInt, ErrorDiff;
	public:
		//�݌v�p�����[�^
		float		fLength[2];			//�ڂ����̒���
		float		fTilt;				//�`���g�p
		Spr::Vec3f	v3BasePosition;		//�x�[�X�ʒu�i�{�f�B�t���[���j
		Spr::Vec3f	v3ToePosition;		//�j���[�g�������̂ܐ�ʒu�i�{�f�B�t���[���j
		Spr::Vec3f	v3DesiredPosition;	//�ܐ�ڕW�ʒu
		float		fDirection;			//�{�f�B���S����ܐ�ւ̕��ʊp
		float		fDistance;			//�{�f�B���S����ܐ�ւ̋���

		Beetle*		pBody;				//���̂ւ̎Q��
		Spr::PHJoint* pJoint[4];		//�֐�
		
		void Reset();
		void SetPosition(const Spr::Vec3f& pos);
		void Step(double dt);
		Leg(){}
		~Leg(){}
	};

	bool	bReady;

	//�݌v�p�����[�^
	float	fPValue, fIValue, fDValue;	//PID�W��
	float	fStride;					//����
	float	fTurnAngle;					//���񎞂̋r�̐U��p
	float	fClearance;					//�r�����グ��
	float	fSpeed;						//���H�ł̑���̑��x
	float	fSwingBackRate;				//���H���x/���H���x

	//��A���A�r
	Spr::PHJoint *pjntNeck[2];
	Spr::PHJoint *pjntTail;
	Leg Legs[2][3];

	//���s����
	BeetleState	nState;
	int		nNeutralStep;
	float	fCounter;
	
	bool ConnectToSpringhead();
	void SetState(BeetleState);
	void Step(double dt);
	void CheckForce();
	Beetle(void);
	~Beetle(void);
};
