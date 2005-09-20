// IHCSpidar4quad.h: IHCSpidar4quad �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IHCSPIDAR4QUAD_H__30CF982A_E5C6_4390_92B0_9558FCB1C11C__INCLUDED_)
#define AFX_IHCSPIDAR4QUAD_H__30CF982A_E5C6_4390_92B0_9558FCB1C11C__INCLUDED_

/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/

#include "SH4Matrix.hpp"

class IHCMotor;
class IHCDeviceManager;

class IHCSpidar4quad
{
protected:
	int state;
public:
	static const int SYSTEM_INIT;
	static const int MOTOR_INIT;
	static const int BEFORE_CALIB;
	static const int CALIB;
	static const int AFTER_CALIB;
	static const int RUN;
	static const int STOP;
	static const int EXIT;
	
public:
	float stiffnessK;
	float dumperB;
	
	Vec3f getForce;	// PC ����󂯎����Force
	Vec3f sendPos;	// PC �ɑ�����Pos

	Vec3f localPos;		// SH�̃��[�J���v�Z�p pos
	Vec3f localForce;	// SH�̃��[�J���v�Z�p Force


public:
	Vec3f posEnc;		//�����O���b�v�̈ʒu�̐���l (�G���R�[�_)
	Vec3f posEncAD;		//�����O���b�v�̈ʒu�̐���l (�G���R�[�_ + A/D)
	Vec3f velEnc;		//�����O���b�v�̑��x�̐���l (�G���R�[�_�̔����l)
	Vec3f velEncAD;		//�����O���b�v�̑��x�̐���l (�G���R�[�_�̔����l + A/D)
	Vec3f accEnc;		//�����O���b�v�̉����x�̐���l (�G���R�[�_�̔����l�̔����l)
	Vec3f accEncAD;		//�����O���b�v�̉����x�̐���l (�G���R�[�_�̔����l�̔����l + A/D�̔����l)

public:
	Vec3f posSqrConst;
	Matrix3f matPos;

	Vec3f phi[4];
	float tension[4];

public:
	static const int motorSize;
	IHCMotor* pMotor[4];

public:
	void SetState(int inState);
	int GetState();
	void SetMinForce();
	void SetMinMinForce();
	IHCDeviceManager* pDevMan;
	void Update(float freqHz);
	void InitMat();
	/*
	void InitMotor(Vec3f* motorPos,float vpn,float lpp,float minT,float maxT);
	void InitMotor2(
		Vec3f *motorPos, 
		float* apn,float* hpa, float* lpp,float* vph, 
		float* minT, float* maxT, 
		char* phs);
		*/
	void InitMotor3();
	void BeforeCalib();
	void Calib();
	void AfterCalib();
	void SetForce(Vec3f& f, float eff = 0.1f, float cont = 0.001f);
	IHCSpidar4quad();
	~IHCSpidar4quad();

protected:
	float iir1filter(float val, float prev, float a);
};

#endif // !defined(AFX_IHCSPIDAR4QUAD_H__30CF982A_E5C6_4390_92B0_9558FCB1C11C__INCLUDED_)
