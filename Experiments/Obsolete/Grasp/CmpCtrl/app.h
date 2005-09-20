#ifndef APP_H
#define APP_H

#include "HRDeviceProxy.h"
#include "HRSolidProxy.h"
#include "TFrame.h"
#include "TShape.h"
#include <WinBasis/WBMMTimer.h>
#include <algorithm>
#include <gl/gl.h>
#include <HIS/HISpidar8.h>
#include <HIS/HISpidar4Quad.h>
#include <fstream>

namespace LVRM{

class TMassDynamics:public HRProxyBase{
protected:
public:
	float mass;
	Vec3f massPos;
	Vec3f massVel;
	Vec3f force;
	TMassDynamics();
	void AddForce(const Vec3f& f){ force += f; }
	void Update();
};
class THapticApp:public UTRefCount{
protected:
	volatile bool bReviseDevPrx;

public:
	Vec3f externalForce;
	Affinef afModel;
	WBMMTimer timer;
	int hapticStepCount;
	int windowsTickCount;
	DWORD windowsTickOffset;
	UTRef<HRSolidProxy> solid;
	UTRef<TFrame> shape;
	TDeviceProxies devices;
	DVDeviceManager devMan;	
	TMassDynamics massDynamics;
#ifdef SPIDAR8
	HISpidar8 sp8;
#else
	HISpidar4quad sp[2];
#endif
	Vec3f gravity;

	std::ofstream* file;
	std::ostream& File(){ return *file; }
//	std::ostream& File(){ return std::cout; }

	THapticApp();
	~THapticApp();
	void Step();
	void Reset();
	void Calib();
	void Draw();
	static void TimerFunc(void* arg);
	void NormalMode();
	void KawaiYoshikawaMode();
	void HaseMode(float* nR, float* pR);
	void StopSave();
	void StartSave();
};

}
#endif
