#ifndef HISPIDARCALC3DOF_H
#define HISPIDARCALC3DOF_H

#include "HISpidarCalcBase.h"
namespace Spr{;

/**	�G���h�G�t�F�N�^(�O���b�v)�����i3���R�x����������SPIDAR�p�̌v�Z�N���X�D
	���̐��͉ρD	*/
class HISpidarCalc3Dof: public HISpidarCalcBase{
protected:
	Vec3f pos;						///<	�O���b�v�̈ʒu
	int nRepeat;							///<	�J��Ԃ���
	std::vector<Vec3f> wireDirection;		///<	���C�������P�ʃx�N�g���i���i�j

	float sigma;							///<	�W���s��̑Ίp�����ɉ�����
public:
	HISpidarCalc3Dof();
	virtual ~HISpidarCalc3Dof();
	void Init(int dof, const VVector<float>& minF, const VVector<float>& maxF);

	virtual void Update();								///<	�p�����̍X�V
	virtual void SetForce(const Vec3f&);				///<	�񎦗͂̐ݒ�i���i�́j
	virtual void SetForce(const Vec3f&, const Vec3f&);	///<	�񎦗͂̐ݒ�i���i�D��]�͂͒񎦂��Ȃ��j
	Vec3f trnForce;		///	���i��
	float smooth;				///	�񎟌`���@���͂̂Q�捀�̌W��
	float lambda_t;				///	�񎟌`���@���i���̌W��
};

}	//	namespace Spr

#endif
