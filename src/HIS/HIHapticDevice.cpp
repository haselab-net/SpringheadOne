#include "HIS.h"
#ifdef _WIN32
 #pragma hdrstop
#endif

namespace Spr {;

HIOBJECTIMPABST(HIForceDevice3D, HIForceDevice6D);

HIForceDevice3D::HIForceDevice3D(){
}

HIOBJECTIMPABST(HIForceDevice6D, HIHapticDevice);
HIForceDevice6D::HIForceDevice6D():alpha(0.90f){
}
void HIForceDevice6D::Update(float dt){
	HIHapticDevice::Update(dt);

	Vec3f pos = GetPos();
	Quaternionf ori = GetOri();
	Vec3f v = (pos - lastPos) / dt;
	Vec3f av = (ori * lastOri.inv()).rotation() / dt;
	vel = alpha*vel + (1-alpha)*v;
	angVel = alpha*angVel + (1-alpha)*av;
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
