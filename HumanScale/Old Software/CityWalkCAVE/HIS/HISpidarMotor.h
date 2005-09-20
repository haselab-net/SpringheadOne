#ifndef VR_HAPTIC_HDSPIDARMOTOR_H
#define VR_HAPTIC_HDSPIDARMOTOR_H

#include "Base/Affine.h"
#include "Device/DVDaBase.h"
#include "Device/DVCounterBase.h"

namespace LVRM {

class HISpidarMotor{
public:
	///	最大出力(N)
	float maxForce;
	///	最小出力(N)
	float minForce;
	///	D/Aの出力 / 印加電圧  (V/N)
	float voltPerNewton;
	///	糸の長さ / パルス数
	float lengthPerPulse;
	///最大出力－最小出力
	float tLimit;
	///	使用するD/A.
	DVDaBase* da;
	///	使用するエンコーダのカウンタ.
	DVCounterBase* counter;
	///	モータが取り付けられている位置(糸の出口)の座標
	Vec3f pos;

public:
	///	
	HISpidarMotor();
	///	
	virtual ~HISpidarMotor(){}
	///	出力の設定
	void Force(float f){
		if(f < minForce) f = minForce;
		if(f > maxForce) f = maxForce;
		if(da) da->Voltage(f * voltPerNewton); 
	}
	///	現在の糸の長さの設定
	void Length(float l){ if(counter) counter->Count(l / lengthPerPulse); }
	///	現在の糸の長さ
	float Length(){
		if(counter) return counter->Count() * lengthPerPulse;
		else return 0;
	}
};

}	//	namespace LVRM
#endif
