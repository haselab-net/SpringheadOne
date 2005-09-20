#include "SpidarInterface.h"
#include <HIS/HISpidarCanoe.h>
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>

using namespace Spr;

const float dt = 0.001f;
DVDeviceManager* devMan;
UTRef<HISpidarCanoe> spidarG6;

extern "C"{
void OnExit();
void SPInit(){
	delete devMan;
	devMan = new DVDeviceManager;
	devMan->RPool().Register(new DRUsb20Simple(10));
	devMan->RPool().Register(new DRUsb20Sh4(0));
	devMan->RPool().Register(new DRUsb20Sh4(1));
	devMan->Init();
	std::cout << *devMan;
	spidarG6 = new HISpidarCanoe;
	spidarG6->Init(*devMan);
	Sleep(1000);
	if (spidarG6->IsGood()) spidarG6->Calib();
	atexit(OnExit);
}
void SPCalib(){
	if (spidarG6 && spidarG6->IsGood()) spidarG6->Calib();
}
void SPUpdate(){
	if (spidarG6->IsGood()) spidarG6->Update(dt);
}
static Vec3f force, torque;
const float MAX_FORCE=7.0f, MAX_TORQUE=7.0f;
void SPSetForce(float x, float y, float z){
	force = Vec3f(x,y,z);
	if (force.norm() > MAX_FORCE){
		force = force.unit() * MAX_FORCE;
	}
	if (spidarG6->IsGood()) spidarG6->SetForce(force, torque);
}
void SPSetTorque(float x, float y, float z){
	torque = Vec3f(x,y,z);
	if (torque.norm() > MAX_TORQUE){
		torque = torque.unit() * MAX_TORQUE;
	}
	if (spidarG6->IsGood()) spidarG6->SetForce(force, torque);
}
float* SPGetPosition(){
	static Vec3f pos;
	if (spidarG6->IsGood()){
		pos = spidarG6->GetPos();
	}else{
		static Vec3f p(0,0,0);
		static float delta = -0.003f;
		if (p.Z() > 0.2) delta = -abs(delta);
		if (p.Z() < -0.2) delta = abs(delta);
		p.Z() += delta;

		p.Y() = (delta>0?-3:10) * (0.2-abs(p.Z())) * 0.1 + 0.05;
		p.X() = abs(p.Z())*0.2;
		pos = p;
	}
	pos.Y() += 0.1f;
	pos *= 0.1;
	return pos.data;
}
float* SPGetOrientation(){
	static Quaternionf ori;
	if (spidarG6->IsGood()){
		ori = spidarG6->GetOri();
	}else{
		ori = Quaternionf::Rot(Rad(30), 'Z');
	}
	return ori.data;
}
float* SPGetRotation(){
	static Matrix3f rot;
	Quaternionf ori;
	ori = *(Quaternionf*)SPGetOrientation();
	ori.to_matrix(rot);
	return (float*)rot.data;
}

void OnExit(){
	for(int i=0; i<10; ++i){
		if (spidarG6->IsGood()){
			spidarG6->SetForce(Vec3f());
			spidarG6->SetMinForce();
			spidarG6->Update();
			Sleep(10);
		}
	}
	spidarG6 = NULL;
	delete devMan;
}

}
