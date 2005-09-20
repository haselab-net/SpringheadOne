#ifndef VR_HAPTIC_HDDIFFCALC_H
#define VR_HAPTIC_HDDIFFCALC_H
#include "HIHapticDevice.h"
#include <Base/RingBuf.h>

namespace LVRM {


class HDDiffCalc{
protected:
	float dt;
public:
	HDDiffCalc(HDForceDevice3D* f=NULL){ fd3 = f; }
	UTRingBuffer<Vec3f> posHist;
	UTRingBuffer<Vec3f> velHist;
	Vec3f accel;
	HDForceDevice3D* fd3;
	void Update();
};

}	//	namespace LVRM
#endif
