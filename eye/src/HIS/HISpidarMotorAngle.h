#include "HISpidarMotor.h"
namespace Spr {

class SPR_DLL HISpidarMotorAngle : public HISpidarMotor
	{
public:
	///�@���̊p�x�@�^�@�p���X��
	float anglePerPulse;
	///�@�p�x�p�̃G���R�[�_�J�E���^
	DVCounterBase* counter_angle;
	///�@�p�x
	float ang;

public:
	///	
	HISpidarMotorAngle();
	///		virtual ~HISpidarMotorAngle(){}


	///�@���݂̎��̊p�x�̐ݒ�
	void Angle(float a){
		if (counter_angle)
			counter_angle -> Count(long(1 / anglePerPulse));
	}

	///���݂̎��̊p�x
	float Angle(){
		if (counter_angle) return
			counter_angle -> Count() * anglePerPulse;
		else return 0;
	}
};
}
