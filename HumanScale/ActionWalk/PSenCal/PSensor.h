#include "Sensor.h"
#include <Device/DVDeviceManager.h>
#include <Device/DVAdBase.h>
#include <Base/Affine.h>

namespace LVRM{;
class PSensor{
public:
	PSensor(){};
	PSensor(int n);
	void Calibration(int calibNum);
	bool CheckSensor();
	void PrintFile(FILE *fp);
	bool exist;
protected:
	DVDeviceManager dev;
	float c1,c2;
	int number;
	Sensor sensor[4];
	float sensorValue;
	float weigh;
};
}
