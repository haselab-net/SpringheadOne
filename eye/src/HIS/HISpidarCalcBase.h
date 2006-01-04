#ifndef HISPIDARCALCBASE_H
#define HISPIDARCALCBASE_H

#include <Base/BaseDebug.h>
#include <Base/Affine.h>
#include <Base/QuadProgram.h>
#include <iostream>
#include <vector>
#include <math.h>

namespace Spr{;
/**	SPIDAR(�����g�����͊o�񎦑��u)�̂��߂̎p���v�Z�Ɨ͊o�v�Z���s��
��{�N���X�D
�C���^�t�F�[�X(���z�֐�)�ƁC
2���v��@��J��Ԃ��@�ɂ��p���v�Z�̂��߂̍�Ɨ̈��p�ӂ���D
void Spr::HISpidarCalcBase::MakeWireVec(), 
void Spr::HISpidarCalcBase::UpdatePos(), 
void Spr::HISpidarCalcBase::MeasureWire(), 
��3�̊֐����I�[�o�[���C�h���邱�ƂŌX��SPIDAR�̂��߂̌v�Z���s���悤��
�ł���D	*/
class SPR_DLL HISpidarCalcBase{
protected:
	/**@name	�p���Ɨ͊o���v�Z���邽�߂̃p�����[�^.
		�p���x�N�g�� = a(���C����) = p(w(���C����))
		a: ���C���� ���� �p���x�N�g�� �ւ̕ϊ��֐��D
		w: ���C���� ���� ���C���ڍ��ʒu�x�N�g�� �ւ̕ϊ��֐��D
		p: ���C���ڍ��ʒu�x�N�g�� ���� �p���x�N�g�� �ւ̕ϊ��֐�

		�p���x�N�g�� = a(���C����) = p(w(���C����)) ��Δ��������
		���p���x�N�g�� = A�E�����C���� = P�EW�E�����C����
	*/
	//@{
	PTM::VVector<float> measuredLength;				///<	�v���������C����
	PTM::VVector<float> calculatedLength;			///<	�O�̎p������v�Z����郏�C����
	PTM::VMatrixRow<float> matA;						///<	�s�� A
	PTM::VMatrixRow<float> matATA;						///<	A * A�̓]�u
	PTM::VVector<float> lengthDiff;					///<	�����C���� = �v�����C���� - �Z�o���C����.		lengthDiff[i] is the measured SetLength of this time minus the computed legth of last time  
	PTM::VVector<float> lengthDiffAve;				///<	�����C�����̕��ϒl
	PTM::VVector<float> postureDiff;				///<	���߂��p���̕ω��� = A * lengthDiff
	//@}
	VQuadProgram<float> quadpr;				///<	�Q���v����̌v�Z�N���X
public:
	HISpidarCalcBase(){}					///<	�R���X�g���N�^
	virtual ~HISpidarCalcBase(){}			///<	�f�X�g���N�^
protected:
	virtual void Update()=0;				///<	�p�����̍X�V
	PTM::VVector<float>& Tension() {		///<	����
		return quadpr.vecX;
	}
	/**	������
		@param minF	�ŏ�����
		@param maxF	�ő咣��
		@param dof	�G���h�G�t�F�N�^�̎��R�x	*/
	void Init(int dof, const VVector<float>& minF, const VVector<float>& maxF);
	virtual void MakeWireVec()=0;			///<	���C�������x�N�g���̌v�Z
	virtual void UpdatePos()=0;				///<	���C���ݒu�ʒu���W�̍X�V
	virtual void MeasureWire()=0;			///<	���C�����̌v��
};

}	//	namespace Spr
#endif
