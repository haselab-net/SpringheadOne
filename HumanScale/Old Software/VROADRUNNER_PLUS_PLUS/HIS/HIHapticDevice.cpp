#include "HIHapticDevice.h"
#include <Device/DRRealDevice.h>
namespace LVRM {

std::set<DRRealDevice*> HIHapticDevice::realDevices;
int HIHapticDevice::deviceUpdateStep;
void HIHapticDevice::AddRealDeviceDependency(DRRealDevice* rd){
	realDevices.insert(rd);
}
void HIHapticDevice::Update(){
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
}	//	namespace LVRM
