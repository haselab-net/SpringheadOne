// CRQuadPRBalance.h: CRQuadPRBalance �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRQUADPR_BALANCE_H
#define CRQUADPR_BALANCE_H

#include <WinBasis/WBPreciseTimer.h>
#include <Base/QuadProgram.h>
#include "CRBalance.h"

#define R_ANKLE_J	29	// �E����̊֐�(�Ō��)�̔ԍ�
#define L_ANKLE_J	37	// ������̊֐�(�Ō��)�̔ԍ�
#define R_HEEL_S	12	// �E�����̔ԍ�
#define L_HEEL_S	16	// �������̔ԍ�
#define ANKLE_DOF 3
#define ANKLE_NUM 2
#define DOF_NUM 6

namespace Spr{;

//�o�����X����邽�߁A�d�S�ɕK�v�ȃg���N���e�֐߂ɍœK���z���ăo�����X���Ƃ�N���X�B
class CRQuadPRBalance : public CRBalance  
{
public:
	CRQuadPRBalance();
	virtual ~CRQuadPRBalance();

	// �N���X�̃C���X�^���X
	VQuadProgram<double> quadPR;								// �񎟌v��@
	WBPreciseTimer timer;										// ���Ԍv��

	//////////�@ �֐��@�@//////////
	void RegistNoJoint(CRHuman* crHuman);						// �œK���Ŏg��Ȃ��֐߂����߂�
	bool CompNoJoint(int jointNum);								// �œK���Ŏg��Ȃ��֐߂��ǂ����𔻒f

	std::vector<int> noJoint;						// �œK���Ŏg��Ȃ��֐ߔԍ�
};
}		//	end namespace Spr
#endif // !defined(CRQUADPR_BALANCE_H)
