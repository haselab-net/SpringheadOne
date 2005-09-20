#ifndef HRSOLIDPROXY_H
#define HRSOLIDPROXY_H
#include "HRProxyBase.h"
#include <Base/RingBuf.h>

namespace LVRM{

class HRObjectProxyBase: public HRProxyBase{
protected:
	Affinef proxyPosture;
	/**
		Ki	バネ定数
		Ri	バネとプロキシの接点
		Pi	バネと力覚提示装置の接点
	*/
	float sumKi;
	Vec3f sumKiRi;
	Vec3f sumKiPi;
	Vec3f sumKiPixRi;
	Matrix3f sumKiPixRix;
public:
	///	プロキシーの姿勢(質量をもたず，すぐにつりあいの位置に移動するプロキシー)
	Affinef& ProxyPosture(){ return proxyPosture; }
	///	物体の姿勢
	virtual Affinef& ObjectPosture()=0;
	///	バネの追加
	virtual void AddSpring(float k, const Vec3f& r, const Vec3f& p)=0;
	///	追加したバネのクリア
	virtual void ClearSprings()=0;
	///	プロキシをつりあい位置に移動
	virtual void UpdateProxy()=0;
	virtual void MoveObject(Vec3f p, Vec3f a)=0;
};

///	剛体を多指で操作する際の剛体のプロキシー
class HRSolidProxy:public HRObjectProxyBase{
protected:
	///	物体の姿勢(質量を持ち，物理に従って動く物体)
	Affinef objectPosture;
	///	プロキシと物体の位置関係．
	UTRingBuffer<Vec3f> trans, rotation;
	///	プロキシと物体を結ぶバネのバネ定数．
	float kTrans, kRotation;
	///	力計算のためのダンパ定数．
	float bTrans, bRotation;
public:
	///
	HRSolidProxy();
	void SetSpringDamper(float kt, float bt, float kr, float br);
	float& KTrans(){ return kTrans; }
	float& KRotation(){ return kRotation; }
	Vec3f& Trans(){ return trans[0]; }
	Vec3f& Rotation(){ return rotation[0]; }
	///	物体の姿勢
	Affinef& ObjectPosture(){ return objectPosture; }
	///	物体の移動
	void MoveObject(Vec3f p, Vec3f a);
	///	物体をプロキシの位置に移動
	void FitObject(){
		trans[0] = Vec3f();
		trans[1] = Vec3f();
//		rotation[0] = Vec3f();
//		rotation[1] = Vec3f();
		objectPosture.Pos() = proxyPosture.Pos();
	}
	///	バネが物体に及ぼす力
	Vec3f Force(){
		return ProxyPosture().Rot() * 
			(kTrans*trans[0] + bTrans*(trans[0]-trans[-1]) / dt);
	}
	///	バネが物体に及ぼすトルク
	Vec3f Torque(){
		return ProxyPosture().Rot() * 
			(kRotation*rotation[0] + bRotation*(rotation[0]-rotation[-1]) / dt);
	}
	///	バネの追加
	void AddSpring(float k, const Vec3f& r, const Vec3f& p);
 	///	追加したバネのクリア
	void ClearSprings();
	///	プロキシをつりあい位置に移動
	void UpdateProxy();
protected:
	///	trans/rotation から つりあいの位置を求める．
	void UpdateProxyPosture();
};

}	//	namespace VRHaptic
#endif
