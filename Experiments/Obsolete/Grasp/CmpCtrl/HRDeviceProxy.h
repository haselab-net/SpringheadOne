#ifndef VR_HRDEVICEPROXY_H
#define VR_HRDEVICEPROXY_H

#include "HRProxyBase.h"
#include "TFrame.h"
#include <Base/utilities.h>
#include <Base/ringbuf.h>
#include <Base/Affine.h>
#include <HIS/HIHapticDevice.h>

namespace LVRM{

class HRDeviceProxy3D: public HRProxyBase{
protected:
	float spring, damper;
	UTRingBuffer<Vec3f> displacements;
	Vec3f proxyPos;
	Vec3f devicePos;
	Vec3f force;
public:
	Vec3f devicePosOrg;
	Vec3f colNormal;
	TShape* collideTo;
	Vec3f colNormalLast;
	TShape* collideToLast;

	Affinef afDevice;
	HDForceDevice3D* device;
	float reviseRatioNormal;
	float reviseRatioParallel;
public:
	HRDeviceProxy3D();
	float& Spring(){ return spring; }
	float& Damper(){ return damper; }

	TShape* CollideTo(){ return collideTo; }
	virtual void SetDevice(class HDForceDevice3D* dev){ device = dev; }
	virtual Vec3f GetDevicePos(){ return devicePos; }
	virtual void SetDevicePos(const Vec3f& p){ devicePos = p; }
	virtual Vec3f GetProxyPos(){ return proxyPos; }
	virtual void SetProxyPos(const Vec3f& p){ proxyPos = p; }
	virtual void CalcForce();
	Vec3f Force(){ return force; }
	virtual void UpdateProxy(TFrame* frame);
	virtual void UpdateDevice();
};

class TDeviceProxies:public	std::vector< UTRef<HRDeviceProxy3D> >{
public:
	volatile int current;
	TDeviceProxies(){ current = 0; }
};

}	//	namespace VRHaptic
#endif
