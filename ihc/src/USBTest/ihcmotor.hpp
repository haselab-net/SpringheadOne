// IHCMotor.h: IHCMotor �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IHCMOTOR_H__57142B1A_A99D_43CC_B41A_93755C7E0013__INCLUDED_)
#define AFX_IHCMOTOR_H__57142B1A_A99D_43CC_B41A_93755C7E0013__INCLUDED_

#include "SH4Vector.hpp"	// ClassView �ɂ���Ēǉ�����܂����B
/*
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
*/

#include "SH4Matrix.hpp"

class IHCMotor  
{
public:
	IHCMotor();

protected:
	unsigned char enc_phs;	// �G���R�[�_����

	volatile float amp;				// �d���l
	float volt;				// �d���l
	
	float offsetLength;		// �����I�t�Z�b�g[m]
	float length;			// ����[m]

	float vel_length;		// �����̑��x[m/s]
	float acc_length;		// �����̉����x[m/s2]
	float dumper0;			// �����׎��̃_���o�[[N/ms-1]

public:

protected:
	// �A���v�萔
	float ampPerNewton;		// [A/N]
	float hexPerAmp;		// [HEX/A]
	float lengthPerPulse;	// [m/Pulse]
	float voltPerHex;		// [V/HEX]

protected:
	float maxForce;			// �ő咣��[N]
	float minForce;			// �ŏ�����[N]
	// ���[�^�萔
	float torqueConst;		// [mNm/A]
	float terminalResist;	// [��]
	float terminalInduct;	// [mH]
	float speedConst;		// [rpm/V]
	float pulleyRadius;		// [mm]

public:
	Vec3f pos;				// ���[�^�̈ʒu(���̏o��)

public:
	float getMinF() {return this->minForce;};
	float getMaxF() {return this->maxForce;};
public:
	Vec3f GetPos();
	void SetPos(float x,float y,float z);
	float GetLength();
	void SetOffsetLength();
	void SetOffsetLength(float offset);
	void SetEncPhs(unsigned char phs);
	unsigned short calcHexDA();
	void calcVolt(short hexAd);
	void calcLength(short hexEnc);
	void calcLength(short hexEnc, short hexAd, float freqHz);
	void SetAmpInfo(float apn,float hpa, float lpp, float vph);
	void SetMotorInfo(float minF,float maxF,float torqueConst,float terminalResist, float terminalInduct,float speedConst,float pulleyRadius);
	void InitForce(float a = 0.5f);
	void Force(float f);
	void printInfo();

};

#endif // !defined(AFX_IHCMOTOR_H__57142B1A_A99D_43CC_B41A_93755C7E0013__INCLUDED_)
