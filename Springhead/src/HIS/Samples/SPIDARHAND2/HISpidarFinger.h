// HISpidarFinger.h: HISpidarFinger クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISPIDARFINGER_H__247EF110_88D3_42C4_B040_A9C35977BF7F__INCLUDED_)
#define AFX_HISPIDARFINGER_H__247EF110_88D3_42C4_B040_A9C35977BF7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HIS/HIHapticDevice.h"
#include "HIS/HISpidarMotor.h"
#include "Device/DVDeviceManager.h"

namespace Spr {

class HISpidarHand;
class HISpidarMotor;

class HISpidarFinger
{
public:
	HISpidarHand* m_pHand;
	HISpidarMotor* m_pMotor1;

	//指先位置
	Vec3f InitFingerPos1;
	Vec3f FingerPos1;
	//指先姿勢
	Vec3f FingerRot1;

	//エンコーダ値格納
	float FingerEnc1[2];
	//(角度/pulse)
	float RadPerPlus1;
	//(長さ/pulse)
	float LengthPerPlus1;
	//角度
	float FingerRad1;
	//長さ
	float FingerLength1;

public:
	virtual void InitInstance(HISpidarHand* pHand,HISpidarMotor* pMotor1,HISpidarMotor* pMotor2);
	HISpidarFinger();
	virtual ~HISpidarFinger();
	//キャリブレーション
	virtual bool BeforeCalib(){SetMinForce();return true;}
	virtual bool FingerCalib();
	virtual bool FingerCalib1();
	//位置計算
	virtual void Calc_FingerPos();
	//張力発生
	virtual void SetMinForce();
	virtual void SetForce(const Vec3f& a){}

	virtual Vec3f GetPos(){return FingerPos1;}
	virtual Vec3f GetRot(){return FingerRot1;}

	virtual void Update();

};

}//namespace Spr 
#endif // !defined(AFX_HISPIDARFINGER_H__247EF110_88D3_42C4_B040_A9C35977BF7F__INCLUDED_)
