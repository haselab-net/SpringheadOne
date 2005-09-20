#ifndef HDSPIDARG6_H
#define HDSPIDARG6_H

#include "HIS/HIHapticDevice.h"
#include "HIS/HISpidarMotor.h"
#include "Device/DVDeviceManager.h"
#include "HISpidarCalc6Dof.h"

namespace Spr {;

class SPR_DLL HISpidarG6: public HDForceDevice6D, HISpidarCalc6Dof{
public:
	std::vector<HISpidarMotor> motor;	///<	���[�^

	HISpidarG6();
	virtual ~HISpidarG6();
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev, int nMotor, const Vec3f(* motorPos)[2], float vpn, float lpp, float minF, float maxF);
	///	�L�����u���[�V�����̑O�ɍs������
	virtual bool BeforeCalib(){ SetMinForce( ); return true; }
	///	�L�����u���[�V����
	virtual bool Calib();
	///	�Œᒣ�͂��o��
	void SetMinForce();
	
	///	�ʒu�̎擾
	Vec3f GetPos(){ return pos; }
	///	�p�x�̎擾
	Quaternionf GetOri(){ return ori; }
	
	///	�͂̐ݒ�
	void SetForce(const Vec3f& f){ SetForce(f, Vec3f()); }
	///	�g���N�̐ݒ�
	void SetForce(const Vec3f& f, const Vec3f& t){ HISpidarCalc6Dof::SetForce(f, t); }
	///	�d�ݕt���̐ݒ�
	void SetWeight(float s=0.3f, float t=1.0f, float r=6.0f){ smooth=s; lambda_t=t; lambda_r=r; }

	///	�͂̎擾
	Vec3f GetForce(){ return Vec3f(); }
	///	�g���N�̎擾
	Vec3f GetTorque(){ return Vec3f(); }

	///	�f�o�C�X�̏�Ԃ��X�V����.
	virtual void Update();

protected:
	virtual void MakeWireVec();					///<	���C�������x�N�g���̌v�Z
	virtual void UpdatePos();					///<	���C���ݒu�ʒu���W�̍X�V
	virtual void MeasureWire();					///<	���C�����̌v��
};

}	//	namespace Spr
#endif
