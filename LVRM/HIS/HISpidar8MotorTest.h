// HISpidar8MotorTest.h: HISpidar8MotorTest �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISPIDAR8MOTORTEST_H__AE02CC6C_28C1_4ED6_A6F2_F0C231F053AC__INCLUDED_)
#define AFX_HISPIDAR8MOTORTEST_H__AE02CC6C_28C1_4ED6_A6F2_F0C231F053AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "HIHapticDevice.h"

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVDeviceManager.h"
#include <vector>


namespace LVRM {

//class HISpidar8MotorTest : public HDForceDevice3D  
class HISpidar8MotorTest : public HIHapticDevice  
{
public:
	HISpidar8MotorTest();
	virtual ~HISpidar8MotorTest();

public:
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev);
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF);
	virtual void Update();
	///	�L�����u���[�V�����̑O�ɍs������
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	�L�����u���[�V����
	virtual bool Calib();
	///	�Œᒣ�͂��o��
	void SetMinForce();
	
	/// �͂̓���
	void SetForce(int ch, float force);
	int GetCount(int ch);

protected:
	///	���[�^
	HISpidarMotor motor[8];

	float realtens[8];	// computed tension + min tension
	float tension[8];	// computed tension

};

}//namespace LVRM

#endif // !defined(AFX_HISPIDAR8MOTORTEST_H__AE02CC6C_28C1_4ED6_A6F2_F0C231F053AC__INCLUDED_)
