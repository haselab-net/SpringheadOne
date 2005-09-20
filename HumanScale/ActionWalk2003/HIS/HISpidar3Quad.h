#ifndef HDSPIDAR3QUAD_H
#define HDSPIDAR3QUAD_H

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVDeviceManager.h"
#include <vector>

namespace LVRM {
using namespace LVRM;

/**	3本糸のSPIDAR.
	糸の長さから座標計算を行ったり、出力する力覚から各モータの
	出力を計算したりする。出力の計算に2次計画法を用いているため，
	HIS/Fortran の libF77.lib libI77.lib QuadPR.lib が必要．
@verbatim
SPIDARの定数の設定例
Init(dev, motPos, 0.5776, 2.924062107079e-5f, 0.5f, 20.0f);

//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
#define PX	0.265f	//	x方向の辺の長さ/2
#define PY	0.265f	//	y方向の辺の長さ/2
#define PZ	0.265f	//	z方向の辺の長さ/2
Vec3f motorPos[][4] =	//	モータの取り付け位置(中心を原点とする){
	{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
	{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
};

//	１ニュートン出すためのDAの出力電圧[V/N]
//	maxon:		0.008[m] / 0.0438[Nm/A] / 0.5[A/V] =  0.365296803653
//	千葉精機:	0.008[m] / 0.0277[Nm/A] / 0.5[A/V] =  0.5776
sp->VoltPerNewton(0.5776f);

//	１カウントの長さ [m/plus]	実測値: 2.924062107079e-5f
sp->LenPerPlus(2.924062107079e-5f);

//	最小張力 [N]
sp->MinForce(0.5f);

//	最大張力 [N]
sp->MaxForce(20.0f);
@endverbatim
*/

class HISpidar3Quad: public HDForceDevice3D{
public:

	typedef HDForceDevice3D superclass;
protected:
	Vec3f pos;
	///	モータ
	HISpidarMotor motor[3];
	///	面のどちら側を使用するか
	bool bLeft;

public:
	///
	HISpidar3Quad();
	///
	virtual ~HISpidar3Quad();
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev);
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF, bool bLeft);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	キャリブレーション
	virtual bool Calib();
	///	簡易キャリブレーション(原点ではなく，モータ3つの重心でキャリブレーション)
	virtual bool EasyCalib();
	///	最低張力を出力
	void SetMinForce();
	///	力の出力
	virtual void SetForce(const Vec3f& f){ SetForce(f, 0.1f); }
	virtual void SetForce(const Vec3f& f, float gamma);
	///	デバイスの状態を更新する.
	virtual void Update();

	Vec3f CalcForce(Vec3f *f);
	
	HISpidarMotor* Motor(){
		return motor;
	}

};


}	//	namespace LVRM
#endif

