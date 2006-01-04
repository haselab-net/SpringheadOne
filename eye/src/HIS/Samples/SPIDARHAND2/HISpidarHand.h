// HISpidarHand.h: HISpidarHand �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISPIDARHAND_H__756B0AE5_3990_41B8_8502_B48FD322CDEC__INCLUDED_)
#define AFX_HISPIDARHAND_H__756B0AE5_3990_41B8_8502_B48FD322CDEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../HISpidarMotor.h"
#include "Device/DVDeviceManager.h"
#include "../../HIHapticDevice.h"

namespace Spr {

class HISpidarFinger;

class HISpidarHand : public HIHapticDevice
{
public:
	///	���[�^
	HISpidarMotor motor[4];

	HISpidarFinger* m_pFinger[5];

	Vec3f HandPos;
	Vec3f HandRot;
//	Vec3f FingerPos;

public:
	HISpidarHand();
	virtual ~HISpidarHand();
	void Update();
	bool InitInstance(DVDeviceManager& dev);
	//�e�w�ʒu�̏����l�w��
	bool Init(Vec3f* FingerPos,float vpn, float lpp, float minF, float maxF);

	virtual Vec3f GetPos(){return HandPos;}
	virtual Vec3f GetRot(){return HandRot;}

};

}//namespace Spr 

#endif // !defined(AFX_HISPIDARHAND_H__756B0AE5_3990_41B8_8502_B48FD322CDEC__INCLUDED_)
