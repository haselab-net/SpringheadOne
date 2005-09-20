#ifndef HDSPIDARG_H
#define HDSPIDARG_H

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVDeviceManager.h"

namespace LVRM {;

class HISpidarG: public HDForceDevice7D{
public:
	typedef HDForceDevice7D superclass;
	///	���[�^
	HISpidarMotor motor[8];
protected:
	PTM::TVector<7, float> F;				//	�ڕW�o��
	double X,Y,Z,X1,Y1,Z1,X2,Y2,Z2,d2;		//	�f�o�C�X�̈ʒu�C�p��
	PTM::TVector<8, float> tension;			//	���̃e���V����
 	PTM::TVector<7, float> FF;				//	�񎦗�
	float smoothnessWeight;					//	�A�����̏d��
	float torqueWeight;						//	�g���N�̏d��
	float graspWeight;						//	�c���͂̏d��

public:
	HISpidarG();
	virtual ~HISpidarG();
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev);
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev, const Vec3f& frame, float vpn, float lpp, float minF, float maxF);
	///	�L�����u���[�V�����̑O�ɍs������
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	�L�����u���[�V����
	virtual bool Calib();
	///	�Œᒣ�͂��o��
	void SetMinForce();
	
	///	�ʒu�̎擾
	Vec3f GetPos();
	///	�p�x�̎擾
	Matrix3f GetRot();
	///	�c�����̊Ԋu�̎擾
	float GetGap();
	
	///	�͂̐ݒ�
	void SetForce(const Vec3f& f, float sw);
	void SetForce(const Vec3f& f){ SetForce(f, 0.1f);}
	///	�g���N�̐ݒ�
 	void SetTorque(const Vec3f& t, float tw);
	void SetTorque(const Vec3f& t){ SetTorque(t, 1.0f);}
	///	�c���͂̐ݒ�
	void SetGrasp(float g, float gw);
	void SetGrasp(float g){ SetGrasp(g, 0.1f);}

	///	�͂̎擾
	Vec3f GetForce();
	///	�g���N�̎擾
 	Vec3f GetTorque();
	///	�c���͂̎擾
	float GetGrasp();

	///	�f�o�C�X�̏�Ԃ��X�V����.
	virtual void Update();
protected:
	bool Pos7dof();		//	�O���b�v�̈ʒu�̌v�Z
	void Force7dof();	//	�񎦒��͂̌v�Z
};

}	//	namespace LVRM
#endif
