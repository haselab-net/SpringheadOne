#include "Sensor.h"

using namespace LVRM;
Sensor::Sensor(){
	sensorValue0 = 0.0f;
	numerator = 0.0f;
	denominator = 0.0f;
	tilt = 0.0f;
}

float Sensor::GetVol(){
	return ad->Voltage();
}