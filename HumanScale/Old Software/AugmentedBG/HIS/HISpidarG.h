#ifndef HDSPIDARG_H
#define HDSPIDARG_H

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVDeviceManager.h"

namespace LVRM {;

class HISpidarG: public HDForceDevice7D{
public:
	typedef HDForceDevice7D superclass;
	///	モータ
	HISpidarMotor motor[8];
protected:
	PTM::TVector<7, float> F;				//	目標出力
	double X,Y,Z,X1,Y1,Z1,X2,Y2,Z2,d2;		//	デバイスの位置，姿勢
	PTM::TVector<8, float> tension;			//	糸のテンション
 	PTM::TVector<7, float> FF;				//	提示力
	float smoothnessWeight;					//	連続性の重み
	float torqueWeight;						//	トルクの重み
	float graspWeight;						//	把持力の重み

public:
	HISpidarG();
	virtual ~HISpidarG();
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev);
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev, const Vec3f& frame, float vpn, float lpp, float minF, float maxF);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	キャリブレーション
	virtual bool Calib();
	///	最低張力を出力
	void SetMinForce();
	
	///	位置の取得
	Vec3f GetPos();
	///	角度の取得
	Matrix3f GetRot();
	///	把持部の間隔の取得
	float GetGap();
	
	///	力の設定
	void SetForce(const Vec3f& f, float sw);
	void SetForce(const Vec3f& f){ SetForce(f, 0.1f);}
	///	トルクの設定
 	void SetTorque(const Vec3f& t, float tw);
	void SetTorque(const Vec3f& t){ SetTorque(t, 1.0f);}
	///	把持力の設定
	void SetGrasp(float g, float gw);
	void SetGrasp(float g){ SetGrasp(g, 0.1f);}

	///	力の取得
	Vec3f GetForce();
	///	トルクの取得
 	Vec3f GetTorque();
	///	把持力の取得
	float GetGrasp();

	///	デバイスの状態を更新する.
	virtual void Update();
protected:
	bool Pos7dof();		//	グリップの位置の計算
	void Force7dof();	//	提示張力の計算
};

}	//	namespace LVRM
#endif
