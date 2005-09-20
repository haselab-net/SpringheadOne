#include "HISpidarMotor.h"
namespace LVRM {

class HISpidarAngle : public HISpidarMotor
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
	HISpidarAngle();
	///		virtual ~HISpidarAngle(){}


	///�@���݂̎��̊p�x�̐ݒ�
	void Angle(float a){
		if (counter_angle)
			counter_angle -> Count(a / anglePerPulse);
	}

	///���݂̎��̊p�x
	float Angle(){
		if (counter_angle) return
			counter_angle -> Count() * anglePerPulse;
		else return 0;
	}
};
}
