#ifndef HISPIDARCALC3DOF_H
#define HISPIDARCALC3DOF_H

#include "HISpidarCalcBase.h"
namespace Spr{;

class HISpidarCalc3Dof: public HISpidarCalcBase{

protected:
	TVec3<float> pos;						///<	�O���b�v�̈ʒu
	int frequency;							///<	�J��Ԃ���
	float sigma;							///<	�W���s��̑Ίp�����ɉ�����

public:
	HISpidarCalc3Dof();
	virtual ~HISpidarCalc3Dof();

	virtual void Update();					///<	�p�����̍X�V
	virtual void SetForce(TVec3<float>);	///<	�񎦗͂̐ݒ�i���i�́j
	virtual void SetForce(TVec3<float>,TVec3<float>);	///<	�񎦗͂̐ݒ�i���i�E��]�͂͒񎦂��Ȃ��j
	TVec3<float> TForce;		///	���i��
	float    smooth;				///	�񎟌`���@���͂̂Q�捀�̌W��
	float    lambda_t;				///	�񎟌`���@���i���̌W��
};

}	//	namespace Spr

#endif
