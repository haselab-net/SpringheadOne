#ifndef HISPIDARCALCBASE_H
#define HISPIDARCALCBASE_H

#include <Base/BaseDebug.h>
#include <Base/Affine.h>
#include "qp_prog01.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <HIS/Fortran/quadpr.h>

namespace Spr{;

class   HISpidarCalcBase{
protected:
	PTM::VVector<float> l_en;				///<	�G���R�[�_�l���瓾���郏�C������
	PTM::VVector<float> l_calc;				///<	�O�̎p������v�Z����郏�C������
	PTM::VMatrix<float> WireMat;			///<	�A���������@WireMat x = VecL
	PTM::VMatrix<float> matATA;				///<	�A���������̌W���s��
	PTM::VVector<float> VecL;				///<	�A��������
	PTM::VVector<float> ATVecL;				///<	AT*VecL
	PTM::VVector<float> se_out;				///<	�A���������̉�
	std::vector<Vec3f>  vec_t;				///<	���C�������P�ʃx�N�g���i���i�j
	std::vector<Vec3f>  vec_r;				///<	���C�������P�ʃx�N�g���i��]���[�����g�j

	///@name fortrun �񎟌v��@�p�ϐ�
	//@{
	PTM::VVector<float> q;
	PTM::VVector<float> cost;
	PTM::VVector<float> ca, rhs, Tq, rc, dual, slk;
	PTM::VVector<long> kt;
	PTM::VVector<float> Tension;
	//@}
	
	//�f�o�C�X�݌v�҂��ݒ肷�鐔
	PTM::VVector<float> minForce;				///<	�ŏ�����
	PTM::VVector<float> maxForce;				///<	�ő咣��

public:
	HISpidarCalcBase(){}						///<	�R���X�g���N�^
	virtual ~HISpidarCalcBase(){}				///<	�f�X�g���N�^
	virtual void Update()=0;					///<	�p�����̍X�V

protected:
	virtual void SetSize(int dof, int nWire);	///<	�����ƃ��C���̖{���ɉ������������̊m��

	virtual void MakeWireVec()=0;				///<	���C�������x�N�g���̌v�Z
	virtual void UpdatePos()=0;					///<	���C���ݒu�ʒu���W�̍X�V
	virtual void MeasureWire()=0;				///<	���C�����̌v��
};

}	//	namespace Spr

#endif
