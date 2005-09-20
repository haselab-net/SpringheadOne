#include <Device/DVDeviceManager.h>
#include <Device/DVAdBase.h>
#include <Base/Affine.h>

namespace LVRM{;
class Sensor{
public:
	Sensor();
	//TTSensor(){};
	float GetVol();
	DVAdBase* ad;
	float sensorValue0;
	float numerator;
	float denominator;
	float tilt;
};
}