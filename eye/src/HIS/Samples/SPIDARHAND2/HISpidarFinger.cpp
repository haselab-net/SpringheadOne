// HISpidarFinger.cpp: HISpidarFinger �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////


#include "HISpidarHand.h"
#include "HISpidarFinger.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr {

HISpidarFinger::HISpidarFinger()
{

}

HISpidarFinger::~HISpidarFinger()
{

}

void HISpidarFinger::InitInstance(HISpidarHand* pHand,HISpidarMotor* pMotor1,HISpidarMotor* pMotor2 = 0)
{
	this->m_pHand = pHand;
	this->m_pMotor1 = pMotor1;
	BeforeCalib();
}

//�Œᒣ�͂̐ݒ�(0.1N)
void HISpidarFinger::SetMinForce()
{
	m_pMotor1->SetForce(0.1f);
}

//�w���90�x�Ȃ��ăL�����u���[�V�����������́A�w����P�O�����L�΂���
bool HISpidarFinger::FingerCalib()
{
	FingerEnc1[0]=m_pMotor1->counter->Count();
	return true;
}

//�w���L�΂��ăL�����u���[�V���� 
bool HISpidarFinger::FingerCalib1()
{
	FingerEnc1[1]=m_pMotor1->counter->Count();

	//���̎w�̈ʒu
	FingerPos1.X()=m_pHand->GetPos().X()+InitFingerPos1.X();
	FingerPos1.Y()=m_pHand->GetPos().Y()+InitFingerPos1.Y();
	FingerPos1.Z()=m_pHand->GetPos().Z()+InitFingerPos1.Z();

	//1pulse������̊p�x������
	RadPerPlus1=(3.141592/2)/(FingerEnc1[0]-FingerEnc1[1]);
	//1pulse������̒���������
	LengthPerPlus1 = 0.05/(FingerEnc1[1]-FingerEnc1[0]);

	return true;
}

void HISpidarFinger::Calc_FingerPos()
{	
	///���݂̃G���R�[�_�l����w�̋Ȃ���p�x��ݒ�
	FingerRad1 = m_pMotor1->counter->Count()*RadPerPlus1;
	///���݂̃G���R�[�_�l���玅�̒�����ݒ�
	FingerLength1 = m_pMotor1->counter->Count()*LengthPerPlus1;

	///�w�̈ʒu�̍X�V
	//X���W
	FingerPos1.X()=m_pHand->GetPos().X()+InitFingerPos1.X();

	//Y���W
	FingerPos1.Y()=m_pHand->GetPos().Y()+InitFingerPos1.Y()-50*sin(FingerRad1);

	//Z���W
	FingerPos1.Z()=m_pHand->GetPos().Z()+InitFingerPos1.Z()-50*cos(FingerRad1);

 }

void HISpidarFinger::Update()
{
	Calc_FingerPos();
}


}//namespace Spr 