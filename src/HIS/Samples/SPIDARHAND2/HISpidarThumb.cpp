// HISpidarThumb.cpp: HISpidarThumb �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "HISpidarThumb.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr {

HISpidarThumb::HISpidarThumb()
{

}

HISpidarThumb::~HISpidarThumb()
{

}

void HISpidarThumb::InitInstance(HISpidarHand *pHand, HISpidarMotor *pMotor1, HISpidarMotor *pMotor2)
{
	this->m_pHand = pHand;
	this->m_pMotor1 = pMotor1;
	this->m_pMotor2 = pMotor2;
}

}//namespace Spr 