#include "HIS.h"
#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr {;

HIOBJECTIMPABST(HIForceDevice3D, HIForceDevice6D);

HIForceDevice3D::HIForceDevice3D(){
}

HIOBJECTIMPABST(HIForceDevice6D, HIHapticDevice);
HIForceDevice6D::HIForceDevice6D():alpha(0.8f){
}
void HIForceDevice6D::Update(float dt){
	HIHapticDevice::Update(dt);

	Vec3f pos = GetPos();
	Quaternionf ori = GetOri();
	Vec3f v = (pos - lastPos) / dt;
	if (v.norm() > 6.0f) v = v.unit() * 6.0f;
	Vec3f av = (ori * lastOri.inv()).rotation() / dt;
	if (av.norm() > 8*M_PI) av = av.unit() * 8*M_PI;
#if 1
	vel = alpha*vel + (1-alpha)*v;
	angVel = alpha*angVel + (1-alpha)*av;
#else
	const int N=5;
	static Vec3f lastVel[N];
	static Vec3f lastAngVel[N];
	for(int i=1; i<N; ++i){
		lastVel[i] = lastVel[i-1];
		lastAngVel[i] = lastAngVel[i-1];
	}
	lastVel[0] = v;
	lastAngVel[0] = av;
	vel.clear();
	angVel.clear();
	for(int i=1; i<N; ++i){
		vel += lastVel[i];
		angVel += lastAngVel[i];
	}
	vel /= N;
	angVel /= N;

#endif
	lastPos = pos;
	lastOri = ori;
}

HIOBJECTIMPABST(HIHapticDevice, HIBase);
void HIHapticDevice::AddRealDeviceDependency(DRRealDevice* rd){
	realDevices.insert(rd);
}
void HIHapticDevice::ClearRealDeviceDependency(){
	realDevices.clear();
}
void HIHapticDevice::Update(float dt){
	updateStep ++;
	if (updateStep > deviceUpdateStep){
		for(std::set<DRRealDevice*>::iterator it = realDevices.begin(); it != realDevices.end(); ++it){
			(*it)->Update();
		}
		deviceUpdateStep = updateStep;
	}else{
		updateStep = deviceUpdateStep;
	}
}

}	//	namespace Spr
