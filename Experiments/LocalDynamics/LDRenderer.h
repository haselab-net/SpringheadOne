#ifndef LDRENDERER_H
#define LDRENDERER_H

//#define USE_IHC
//#define ADJUST_POS

#ifdef USE_IHC
#include <Base/Affine.h>
#include <HIS/HISpidar4.h>
#else
#include <WinBasis/WinBasis.h>
#include <Framework/FWPointer.h>
#endif

#include <HIS/HIHapticDevice.h>
#include "Matrix6.h"
#include "Vector6.h"
#include "../../ihc/src/IHCBase/IHUsbPacket.h"

namespace Spr {;

class LDRenderer{
	float k, b;                      // 物体のばね係数とダンパ係数
	float x, prevX;                  // ポインタの侵入量と一回前の侵入量

	Vec3f torque_ex;                 // 物体のトルク
	Vec3f f_external;                // 外力情報
	Vec3f pos, prevPos;              // 物体の最近傍点の位置と一回前の位置
	Vec3f currentPos;                // 現在の位置（LD計算なし用）
	Vec3f center;                    // 物体の質量中心
	Vec3f normal, prevNormal;        // 衝突の法線と一回前の法線
	Vec3f currentNormal;             // 現在の法線（LD計算なし用）

	Matrix6f Iinv;
	Vec6f    velocities;
	Vec6f    displacement;

	Vec3f impulse;
	Vec3f force;                     // 提示力
	Vec3f pointer, prevPointer;      // ポインタの現在位置と一回前の位置
	float ldDT, simuDT;              // 時間間隔(rendering & simulator)

	bool calc_ld;                    // 局所物理の計算を行うかどうか
	bool bCalc;
	bool bExperiment;				// 実験用フラグ（LDなしの時の場合わけ用）

	float posScale;
	float forceScale;
	
	Quaternionf qtOffset;
	Quaternionf qtOffsetInv;
	Vec3f       v3Offset;

#ifndef USE_IHC
	WBCriticalSection cs;            // Critical Section (Simulatorとの同期）
#endif

public:

#ifndef USE_IHC
	FWPointer6D* device;
#else
//	UTRef<HIForceDevice6D> device;
	HISpidar4* device;
#endif

	LDRenderer();
	~LDRenderer();
	//毎回の更新処理
	void Step();
	// 前回の変数を記録
	void RegisterPreviousParameters();
	// 力覚提示プロセス
	void LDHapticRendering();
	// 提示力の計算
	void CalcForce();
	// 局所的動力学の計算プロセス
	void LDLocalDynamics();
	// 局所的動力学を計算しない場合（比較実験用）
	void LocalUpdate();
	// Simulatorから受け取る位置の修正
	void AdjustPos();
#ifndef USE_IHC
	// Enter Critical Section
	void CSEnter(){ cs.Enter(); }
	// Leave Critical Section
	void CSLeave(){ cs.Leave(); }
#endif

	Vec3f GetImpulse()		{ return impulse;  }
	Vec3f GetPos()			{ return pos;        }
	Vec3f GetNormal()		{ return normal;     }
	Vec3f GetPointer()		{ return pointer;    }
	Vec3f GetForce()		{ return force;      }
	Vec6f GetVelocities()	{ return velocities; }
	Vec6f GetDisplacement()	{ return displacement; }
	float GetLDDT()			{ return ldDT;       }

	void SetSimuDT(float t){ simuDT = t; }
	void SetLDDT(float t)  { ldDT   = t; }
	void SetK (float x)    { k      = x; }
	void SetB (float x)    { b      = x; }
 
	void SetPos       (const Vec3f& p) { pos        = p; }
	void SetNormal    (const Vec3f& n) { normal     = n; }
	void SetCenter    (const Vec3f& c) { center     = c; }
	void SetExTorque  (const Vec3f& t) { torque_ex  = t; }
	void SetExForce   (const Vec3f& f) { f_external = f; }
	void SetVelocities(const Vec6f& v) { velocities = v; }

	void SetCurrentPos(){currentPos = prevPos; }

	void SetIinv    (const Matrix6f& i) { Iinv = i; }

	void SetParameters(IHPacketLD packet);
	void ResetParameters(){ impulse.clear(); 
						displacement.clear(); }

	bool GetCalcLD()       { return calc_ld;  }
	void SetCalcLD(bool b) { calc_ld = b; } 
	void SetBCalc (bool b) { bCalc   = b; }
	void SetBExperiment(bool b) { bExperiment = b; }

	void SetPointer(Vec3f p){ pointer = p; }
};

} // namespase Spr

#endif //LDRENDERER_H

