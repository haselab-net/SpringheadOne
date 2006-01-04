// HISpidarHand.cpp: HISpidarHand �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "HISpidarHand.h"
#include "HISpidarFinger.h"
#include "HISpidarThumb.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
namespace Spr {

HISpidarHand::HISpidarHand()
{
	m_pFinger[0] = (HISpidarFinger*)new HISpidarThumb();
	for(int i=1;i<5;i++){
		m_pFinger[i] = new HISpidarFinger();
	}
}

HISpidarHand::~HISpidarHand()
{
	for(int i=0;i<5;i++){
		delete m_pFinger[i];
	}
}

bool HISpidarHand::InitInstance(DVDeviceManager& dev)
{
	/*	*/

	//	�h���C�o�̎擾
	int i;
	for(i=0; i<4; ++i){
		motor[i].da = (DVDaBase*)dev.VPool().Rent("D/A");
		if (!motor[i].da) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<4) return false;
	for(i=0; i<4; ++i){
		motor[i].counter = (DVCounterBase*)dev.VPool().Rent("Counter");
		if (!motor[i].counter) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	if (i<4) return false;

	// ���[�^��Finger�ɓo�^
	m_pFinger[0]->InitInstance(this,motor,motor+1);
	for(i=1;i<3;i++){
		m_pFinger[i]->InitInstance(this,motor+i+1,NULL);
	}
	return true;
}

//�w��̋Ȃ��n�߈ʒu,���[�^�ϐ���o�^
bool HISpidarHand::Init(Vec3f* FingerPos,float vpn, float lpp, float minF, float maxF)
{
	for(int i=0;i<4;i++){
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
		motor[i].minForce = minF;
		motor[i].maxForce = maxF;
	}

	for(int i=0; i<3; i++){
		m_pFinger[i]->InitFingerPos1 = FingerPos[i];
		m_pFinger[i]->BeforeCalib();
	}

	return true;
}

void HISpidarHand::Update()
{
	HIHapticDevice::Update();
}

} //namespace Spr 
