#ifndef HRSOLIDPROXY_H
#define HRSOLIDPROXY_H
#include "HRProxyBase.h"
#include <Base/RingBuf.h>

namespace LVRM{

class HRObjectProxyBase: public HRProxyBase{
protected:
	Affinef proxyPosture;
	/**
		Ki	�o�l�萔
		Ri	�o�l�ƃv���L�V�̐ړ_
		Pi	�o�l�Ɨ͊o�񎦑��u�̐ړ_
	*/
	float sumKi;
	Vec3f sumKiRi;
	Vec3f sumKiPi;
	Vec3f sumKiPixRi;
	Matrix3f sumKiPixRix;
public:
	///	�v���L�V�[�̎p��(���ʂ��������C�����ɂ肠���̈ʒu�Ɉړ�����v���L�V�[)
	Affinef& ProxyPosture(){ return proxyPosture; }
	///	���̂̎p��
	virtual Affinef& ObjectPosture()=0;
	///	�o�l�̒ǉ�
	virtual void AddSpring(float k, const Vec3f& r, const Vec3f& p)=0;
	///	�ǉ������o�l�̃N���A
	virtual void ClearSprings()=0;
	///	�v���L�V���肠���ʒu�Ɉړ�
	virtual void UpdateProxy()=0;
	virtual void MoveObject(Vec3f p, Vec3f a)=0;
};

///	���̂𑽎w�ő��삷��ۂ̍��̂̃v���L�V�[
class HRSolidProxy:public HRObjectProxyBase{
protected:
	///	���̂̎p��(���ʂ������C�����ɏ]���ē�������)
	Affinef objectPosture;
	///	�v���L�V�ƕ��̂̈ʒu�֌W�D
	UTRingBuffer<Vec3f> trans, rotation;
	///	�v���L�V�ƕ��̂����ԃo�l�̃o�l�萔�D
	float kTrans, kRotation;
	///	�͌v�Z�̂��߂̃_���p�萔�D
	float bTrans, bRotation;
public:
	///
	HRSolidProxy();
	void SetSpringDamper(float kt, float bt, float kr, float br);
	float& KTrans(){ return kTrans; }
	float& KRotation(){ return kRotation; }
	Vec3f& Trans(){ return trans[0]; }
	Vec3f& Rotation(){ return rotation[0]; }
	///	���̂̎p��
	Affinef& ObjectPosture(){ return objectPosture; }
	///	���̂̈ړ�
	void MoveObject(Vec3f p, Vec3f a);
	///	���̂��v���L�V�̈ʒu�Ɉړ�
	void FitObject(){
		trans[0] = Vec3f();
		trans[1] = Vec3f();
//		rotation[0] = Vec3f();
//		rotation[1] = Vec3f();
		objectPosture.Pos() = proxyPosture.Pos();
	}
	///	�o�l�����̂ɋy�ڂ���
	Vec3f Force(){
		return ProxyPosture().Rot() * 
			(kTrans*trans[0] + bTrans*(trans[0]-trans[-1]) / dt);
	}
	///	�o�l�����̂ɋy�ڂ��g���N
	Vec3f Torque(){
		return ProxyPosture().Rot() * 
			(kRotation*rotation[0] + bRotation*(rotation[0]-rotation[-1]) / dt);
	}
	///	�o�l�̒ǉ�
	void AddSpring(float k, const Vec3f& r, const Vec3f& p);
 	///	�ǉ������o�l�̃N���A
	void ClearSprings();
	///	�v���L�V���肠���ʒu�Ɉړ�
	void UpdateProxy();
protected:
	///	trans/rotation ���� �肠���̈ʒu�����߂�D
	void UpdateProxyPosture();
};

}	//	namespace VRHaptic
#endif
