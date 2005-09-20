// IHCMotor.h: IHCMotor クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IHCMOTOR_H__57142B1A_A99D_43CC_B41A_93755C7E0013__INCLUDED_)
#define AFX_IHCMOTOR_H__57142B1A_A99D_43CC_B41A_93755C7E0013__INCLUDED_

#include "SH4Vector.hpp"	// ClassView によって追加されました。
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
	unsigned char enc_phs;	// エンコーダ相順

	volatile float amp;				// 電流値
	float volt;				// 電圧値
	
	float offsetLength;		// 糸長オフセット[m]
	float length;			// 糸長[m]

	float vel_length;		// 糸長の速度[m/s]
	float acc_length;		// 糸長の加速度[m/s2]
	float dumper0;			// 無負荷時のダンバー[N/ms-1]

public:

protected:
	// アンプ定数
	float ampPerNewton;		// [A/N]
	float hexPerAmp;		// [HEX/A]
	float lengthPerPulse;	// [m/Pulse]
	float voltPerHex;		// [V/HEX]

protected:
	float maxForce;			// 最大張力[N]
	float minForce;			// 最小張力[N]
	// モータ定数
	float torqueConst;		// [mNm/A]
	float terminalResist;	// [Ω]
	float terminalInduct;	// [mH]
	float speedConst;		// [rpm/V]
	float pulleyRadius;		// [mm]

public:
	Vec3f pos;				// モータの位置(糸の出口)

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
