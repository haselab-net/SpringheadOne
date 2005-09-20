// IHCSpidar4quad.h: IHCSpidar4quad クラスのインターフェイス
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
	
	Vec3f getForce;	// PC から受け取ったForce
	Vec3f sendPos;	// PC に送ったPos

	Vec3f localPos;		// SHのローカル計算用 pos
	Vec3f localForce;	// SHのローカル計算用 Force


public:
	Vec3f posEnc;		//物理グリップの位置の推定値 (エンコーダ)
	Vec3f posEncAD;		//物理グリップの位置の推定値 (エンコーダ + A/D)
	Vec3f velEnc;		//物理グリップの速度の推定値 (エンコーダの微分値)
	Vec3f velEncAD;		//物理グリップの速度の推定値 (エンコーダの微分値 + A/D)
	Vec3f accEnc;		//物理グリップの加速度の推定値 (エンコーダの微分値の微分値)
	Vec3f accEncAD;		//物理グリップの加速度の推定値 (エンコーダの微分値の微分値 + A/Dの微分値)

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
