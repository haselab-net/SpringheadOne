#ifndef HAPTIC_HDSPIDAR8_H
#define HAPTIC_HDSPIDAR8_H
#include "HISpidar3Quad.h"

namespace LVRM{

class HISpidar8{
public:
	HISpidar3Quad sp3[8];
	void Init(DVDeviceManager& devMan);
	static void RegisterDevice(DVDeviceManager& devMan);
	void Update();
};

}	//	namespace Haptic
#endif
