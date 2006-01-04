#ifndef HDSPIDARFISHING_H
#define HDSPIDARFISHING_H

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVPioBase.h"
#include "Device/DVDeviceManager.h"
#include "HISpidarCalc3Dof.h"

namespace Spr {;

/**	@JA	�G���h�G�t�F�N�^(�O���b�v)�����̂�6���R�x��SPIDAR�D���̖{���͉ρD
	@EN	A SPIDAR with a 6DOF solid grip. The number of the strings is variable.@@*/
class SPR_DLL HISpidarFishing: public HIForceDevice3D,public HISpidarCalc3Dof{
public:
	std::vector<HISpidarMotor> motor;	///<	���[�^
	std::vector<DVPioBase*> button;	///<	�{�^��

		///	�f�o�C�X�̃^�C�v
	HIOBJECTDEF(HISpidarFishing);

	HISpidarFishing();
	bool Init(DVDeviceManager& dev);
	virtual double  GetLength(int);

	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
//	bool Init(DVDeviceManager& dev, int nMotor, const Vec3f(* motorPos)[2], float vpn, float lpp, float minF, float maxF);
	///	�L�����u���[�V�����̑O�ɍs������
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	�L�����u���[�V����
	virtual bool Calib();
	///	�Œᒣ�͂��o��
	void SetMinForce();
	
	///	�ʒu�̎擾
	Vec3f GetPos(){ return pos; }
	///	�p�x�̎擾
//	Quaternionf GetOri(){ return ori; }
	
	///	�͂̐ݒ�
	void SetForce(const Vec3f& f){ HISpidarCalc3Dof::SetForce(f); }
	///	�g���N�̐ݒ�
//	void SetForce(const Vec3f& f, const Vec3f& t){ HISpidarCalc6Dof::SetForce(f, t); }
	///	�d�ݕt���̐ݒ�
	void SetWeight(float s=0.3f, float t=1.0f){ smooth=s; lambda_t=t; }

	///	�͂̎擾
	Vec3f GetForce(){ return trnForce; }
	///	�g���N�̎擾
//	Vec3f GetTorque(){ return rotForce; }

	/// �{�^�����̎擾
	int HISpidarFishing::GetButton(int ch){
		if(button[ch]) return button[ch]->Get();
		else return 0;
	}

	///	�f�o�C�X�̏�Ԃ��X�V����.
	virtual void Update(float dt);
	virtual void Update(){ HISpidarCalc3Dof::Update(); }

protected:
	virtual void MakeWireVec();					///<	���C�������x�N�g���̌v�Z
	virtual void UpdatePos();					///<	���C���ݒu�ʒu���W�̍X�V
	virtual void MeasureWire();					///<	���C�����̌v��
};

}	//	namespace Spr
#endif



/*#ifndef HDSPIDARFISHING_H
#define HDSPIDARFISHING_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/

/*
class SPR_DLL HISpidarFishing: public HISpidarG6{
public:
	///	�f�o�C�X�̃^�C�v
	HIOBJECTDEF(HISpidarFishing);

	HISpidarFishing();
	bool Init(DVDeviceManager& dev);
	virtual double  GetLength(int);

};

}	//	namespace Spr
#endif

*/