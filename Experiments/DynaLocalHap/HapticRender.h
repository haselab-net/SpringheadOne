#ifndef HAPTICRENDER_H
#define HAPTICRENDER_H


#include <Base/Affine.h>
#include <HIS/HIHapticDevice.h>
#include "../../ihc/src/IHCBase/IHUsbPacket.h"

using namespace Spr;
#define USE_IHC

class HapticRender{
	float forceScale, posScale;
	Vec3f dSpring, lastSpring;
public:
	typedef IHPacketPlane::PlaneState PlaneState;

	UTRef<HIForceDevice6D> device;
	volatile PlaneState planeState;
	typedef PTM::TVector<3, volatile float> VVec3f;
	VVec3f offset;
	VVec3f force;			//	提示した力積
	VVec3f pointer;			//	デバイスの位置
	VVec3f pos;				//	現在の提示面の位置
	VVec3f normal;			//	現在の提示面の法線の向き
	VVec3f newPos;			//	受け取った提示面の位置
	VVec3f newNormal;		//	受け取った提示面の法線の向き
	VVec3f proxy;			//	プロキシの位置
	float hapticDt;			//	力覚ループのdt
	float simulationDt;		//	シミュレーションのdt
	float hapticTime;

	HapticRender();
	virtual ~HapticRender();
	///	面をセット
	void SetPlane(Vec3f pos, Vec3f normal, PlaneState stat, float ps, float fs, Vec3f offset);
	///	力積を受け取る
	Vec3f GetForce();
	///	
	void HapticStep();

};

#endif
