#include "HISpidarCalc3Dof.h"
namespace Spr{;

class HISpidarCalc6Dof : public HISpidarCalc3Dof {

public:
	HISpidarCalc6Dof();
	virtual ~HISpidarCalc6Dof();

protected:
	Quaternionf ori;
public:
	void Update();								///<	�p�����̍X�V
	void SetForce(TVec3<float>);				///<	�񎦗͂̐ݒ�i���i�̂݁j
	void SetForce(TVec3<float>,TVec3<float>);	///<	�񎦗͂̐ݒ�i���i�E��]�́j
	TVec3<float> RForce;		///��]��
	float lambda_r;		///�񎟌`���@��]���̌W��
	float* InputForTest(float,float,float,float*,float); //�e�X�g�p���͊֐�
};

}	//	namespace Spr
