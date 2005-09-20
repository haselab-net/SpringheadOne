#include "HITurnTable.h"
#include <WinBasis/WBWin.h>
#include <iostream>

namespace LVRM{

const float V_CENTER=0.5f;
const float V_FULLSCALE=2.5f;
HITurnTable::HITurnTable(){
	oriSensor = NULL;
	da = NULL;
	dwTime = 0;
	nowAngle = 0;
	startAngle = 0;
	turnVel = 0;
	angle = 0;
	bControl = true;
	printTurnData = false;
	velocity = (0.0f,0.0f,0.0f);
	stopspeed = 0.5f; //�K��
	eyeAngle = 0.0f;
	isWalking = false;
}

HITurnTable::~HITurnTable(){
}

void HITurnTable::Stop(){
	bControl = false;
}
void HITurnTable::Start(){
	bControl = true;
}

bool HITurnTable::Init(DVDeviceManager& dev){
	dev.VPool().Rent(oriSensor);
	if (!oriSensor) return false;
	dev.VPool().Rent(da);
	if (!da){
		dev.VPool().Return(oriSensor);
		oriSensor = NULL;
		return false;
	}
	startAngle = oriSensor->GetEuler().Z();
	da->Voltage(V_FULLSCALE * V_CENTER);
	dwTime = GetTickCount();
	return true;
}
void HITurnTable::Step(float inner){
	dwLastTime = dwTime;
	dwTime = timeGetTime();
	float dt = float(dwTime - dwLastTime) / 1000;
	///�g�[�L���Z���T���g���Ďg�p�҂̌��������o����ꍇ
	if (oriSensor) {
		nowAngle = -(oriSensor->GetEuler().Z() - startAngle);
	}
	float tokinAngle = oriSensor->GetEuler().Z();
/*
	std::cout << "Tokin exit? : " << (oriSensor != NULL) << std::endl;
	std::cout << "nowAngle by Tokin StartAngle : " << startAngle << std::endl;
	std::cout << "nowAngle by Tokin oriSensorAngle : " << tokinAngle << std::endl;
*/	

	///�g�[�L���Z���T�ƈ��̓Z���T�̌��o�f�[�^���r����ۂ̃f�[�^
	printOriAngle = nowAngle;
	printPressureAngle = inner;
//	std::cout << "nowAngle by Tokin : " << printOriAngle << std::endl;


	///���̓Z���T���g���Ďg�p�҂̌��������o����ꍇ
	if (oriSensor){ 
		nowAngle = inner;
//		std::cout << "nowAngle:" << nowAngle << std::endl;
	}

#ifdef USING_SLC
	if (velocity < stopspeed) { //stopspeed�̒l�͎������Č��߂�D
//	if (!isWalking) {
		nowAngle = 0.0f;
	}
	else{
		nowAngle = inner;
	}
#endif

	const float acc = 2.0f;	//	[rad/sec^2]
	const float k = 2.0f;
	const float gap = 5;
	if(nowAngle > Rad(gap)){
		if(turnVel > - (k * nowAngle))
			turnVel -= acc*dt;
		if(turnVel < - (k * nowAngle))
			turnVel += acc*dt;
	}else if(nowAngle < -Rad(gap)){
		if(turnVel < - (k * nowAngle))
			turnVel += acc*dt;
		if(turnVel > - (k * nowAngle))
			turnVel -= acc*dt;
	}else if(turnVel < 0){
		turnVel += acc*dt;
		if (turnVel > 0) turnVel = 0;
	}else if(turnVel > 0){
		turnVel -= acc*dt;
		if (turnVel < 0) turnVel = 0;
	}
/*	2.5v �̂Ƃ��A�^�[���e�[�u���͎��v����19.2[rpm]
	0v �̂Ƃ��A�^�[���e�[�u���͔����v����19.3[rpm]
	1.25v �� 19.2[rpm] = 2.01	[rad/sec]
	0.622 [V/rad]
*/
	if (turnVel > 1.0f) turnVel = 1.0f;
	if (turnVel < -1.0f) turnVel = -1.0f;
	const float voltPerRad = 0.622f;
	//const float voltPerRad = 1.244f;
	//const float voltPerRad = 1.000f;
	if (bControl){
		if (da) da->Voltage((turnVel * voltPerRad) +1.25);
	}else{
		da->Voltage(1.25);
	}
	angle += turnVel * dt;
}

void HITurnTable::Reset(){
	if (oriSensor){
		startAngle = oriSensor->GetEuler().Z();
//		startAngle = 3.141592 / 2.0;
	}
}

///�g�[�L���Z���T�ƈ��̓Z���T���猟�o���ꂽ�g�p�҂̌����̃f�[�^���o��
void HITurnTable::PrintData(std::ostream& os) const{
	os << printOriAngle << "," << printPressureAngle << "," << dwTime << std::endl;
}

}
