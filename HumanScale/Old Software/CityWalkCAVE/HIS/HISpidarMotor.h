#ifndef VR_HAPTIC_HDSPIDARMOTOR_H
#define VR_HAPTIC_HDSPIDARMOTOR_H

#include "Base/Affine.h"
#include "Device/DVDaBase.h"
#include "Device/DVCounterBase.h"

namespace LVRM {

class HISpidarMotor{
public:
	///	�ő�o��(N)
	float maxForce;
	///	�ŏ��o��(N)
	float minForce;
	///	D/A�̏o�� / ����d��  (V/N)
	float voltPerNewton;
	///	���̒��� / �p���X��
	float lengthPerPulse;
	///�ő�o�́|�ŏ��o��
	float tLimit;
	///	�g�p����D/A.
	DVDaBase* da;
	///	�g�p����G���R�[�_�̃J�E���^.
	DVCounterBase* counter;
	///	���[�^�����t�����Ă���ʒu(���̏o��)�̍��W
	Vec3f pos;

public:
	///	
	HISpidarMotor();
	///	
	virtual ~HISpidarMotor(){}
	///	�o�͂̐ݒ�
	void Force(float f){
		if(f < minForce) f = minForce;
		if(f > maxForce) f = maxForce;
		if(da) da->Voltage(f * voltPerNewton); 
	}
	///	���݂̎��̒����̐ݒ�
	void Length(float l){ if(counter) counter->Count(l / lengthPerPulse); }
	///	���݂̎��̒���
	float Length(){
		if(counter) return counter->Count() * lengthPerPulse;
		else return 0;
	}
};

}	//	namespace LVRM
#endif
