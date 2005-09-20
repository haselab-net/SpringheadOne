#include "HRDeviceProxy.h"
#include "TShape.h"

namespace LVRM{

HRDeviceProxy3D::HRDeviceProxy3D(){
	spring = 1;
	damper = 1;
	reviseRatioNormal = 0;
	reviseRatioParallel = 0;
	displacements.size(2);
	device = NULL;
	collideTo = NULL;
	collideToLast = NULL;
}

void HRDeviceProxy3D::CalcForce(){
	displacements.inc();
	displacements[0] = (devicePos - proxyPos);
	force = spring * displacements[0] + damper * (displacements[0] - displacements[-1]);
}
void HRDeviceProxy3D::UpdateDevice(){
	if (device){
		device->SetForce(-force);
		device->Update();
		devicePos = (Vec3f&)devicePosOrg = afDevice * device->GetPos();
	}else{
		devicePos = (Vec3f&)devicePosOrg;
	}
}

void HRDeviceProxy3D::UpdateProxy(TFrame* frame){
	Vec3f newPrx;
	collideToLast = collideTo;
	colNormalLast = colNormal;
	collideTo = frame->CalcProxyPos(newPrx, colNormal, proxyPos, devicePos);
	if (collideTo) proxyPos = newPrx;
	else proxyPos = devicePos;
}

}
