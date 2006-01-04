// CRQuadPRBalance.cpp: CRQuadPRBalance �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "CRQuadPRBalance.h"
#include "CRHuman.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRQuadPRBalance::CRQuadPRBalance()
{

}

CRQuadPRBalance::~CRQuadPRBalance()
{

}
void  CRQuadPRBalance::RegistNoJoint(CRHuman* crHuman){
	noJoint.clear();
	// �㔼�g
	//	(���j
	noJoint.push_back(3);
	noJoint.push_back(4);
	noJoint.push_back(5);

	//	(��j
	noJoint.push_back(6);
	noJoint.push_back(7);
	noJoint.push_back(8);

	//	(�E���)
	noJoint.push_back(14);
	noJoint.push_back(15);

	//	(�����)
	noJoint.push_back(21);
	noJoint.push_back(22);

	// �����g
	//	(�E����)
	noJoint.push_back(27);
	noJoint.push_back(28);
	noJoint.push_back(29);
	//	(�E�y���܂�)
	noJoint.push_back(30);

	//	(������)
	noJoint.push_back(35);
	noJoint.push_back(36);
	noJoint.push_back(37);

	//	(���y���܂�)
	noJoint.push_back(38);

}
bool CRQuadPRBalance::CompNoJoint(int jointNum){
	for(int i = 0; i < noJoint.size(); i++){
		if(jointNum == noJoint[i])
			return true;
	}
	return false;
}

}		//	end namespace Spr