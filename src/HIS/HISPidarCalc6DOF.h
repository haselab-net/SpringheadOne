#ifndef HISPIDARCALC6DOF_H
#define HISPIDARCALC6DOF_H
#include "HISpidarCalc3Dof.h"
namespace Spr{;

/**	6���R�x��SPIDAR�p�̌v�Z�N���X�D���̖{���͉�	*/
class HISpidarCalc6Dof : public HISpidarCalc3Dof {

protected:
	Quaternionf ori;
	std::vector<Vec3f> wireMoment;				///<	���C�������P�ʃx�N�g���i��]���[�����g�j

public:
	HISpidarCalc6Dof();
	virtual ~HISpidarCalc6Dof();
	///	������
	void Init(int dof, const VVector<float>& minF, const VVector<float>& maxF);
	void Update();								///<	�p�����̍X�V
	void SetForce(const Vec3f&);				///<	�񎦗͂̐ݒ�i���i�̂݁j
	void SetForce(const Vec3f&,const Vec3f&);	///<	�񎦗͂̐ݒ�i���i�E��]�́j
	Vec3f rotForce;								///<	��]��
	float lambda_r;								///<	�񎟌`���@��]���̌W��
	float* InputForTest(float,float,float,float*,float); //�e�X�g�p���͊֐�
};

}	//	namespace Spr
#endif
