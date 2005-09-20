#include "HITurnTableSmall.h"
#include <WinBasis/WBWin.h>
#include <iostream>

namespace LVRM{

const float V_CENTER=0.5f;
const float V_FULLSCALE=2.5f;
HITurnTableSmall::HITurnTableSmall(){
	oriSensor = NULL;
	da = NULL;
	dwTime = 0;
	nowAngle = 0;
	startAngle = 0;
	turnVel = 0;
	angle = 0;
	bControl = true;
	printTurnData = false;
}

HITurnTableSmall::~HITurnTableSmall(){
}

void HITurnTableSmall::Stop(){
	bControl = false;
}
void HITurnTableSmall::Start(){
	bControl = true;
}

bool HITurnTableSmall::Init(DVDeviceManager& dev){
//	dev.VPool().Rent(oriSensor);
//	if (!oriSensor) return false;
	dev.VPool().Rent(da);
	if (!da){
//		dev.VPool().Return(oriSensor);
//		oriSensor = NULL;
		return false;
	}
	oriSensor = NULL;
//	startAngle = oriSensor->GetEuler().Z();
	da->Voltage(V_FULLSCALE * V_CENTER);
	dwTime = GetTickCount();
	return true;
}
void HITurnTableSmall::Step(float inner){
	dwLastTime = dwTime;
	dwTime = GetTickCount();
	float dt = float(dwTime - dwLastTime) / 1000;

	///圧力センサを使って使用者の向きを検出する場合
		nowAngle = inner;
		std::cout << "nowAngle:" << nowAngle << std::endl;

	const float acc = 2.0f;	//	[rad/sec^2]
	const float k = 2.0f;
	const float gap = 10;
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
/*	2.5v のとき、ターンテーブルは時計回りに19.2[rpm]
	0v のとき、ターンテーブルは反時計回りに19.3[rpm]
	1.25v で 19.2[rpm] = 2.01	[rad/sec]
	0.622 [V/rad]
*/
	if (turnVel > 1.0f) turnVel = 1.0f;
	if (turnVel < -1.0f) turnVel = -1.0f;
//	const float voltPerRad = 0.622f;
	const float voltPerRad = 1.244f;
	if (bControl){
		if (da) da->Voltage((turnVel * voltPerRad) +1.25);
	}else{
		da->Voltage(1.25);
	}
	angle += turnVel * dt;
}

void HITurnTableSmall::Reset(){
	if (oriSensor){
		startAngle = oriSensor->GetEuler().Z();
//		startAngle = 3.141592 / 2.0;
	}
}

///トーキンセンサと圧力センサから検出された使用者の向きのデータを出力
void HITurnTableSmall::PrintData(std::ostream& os) const{
	os << printOriAngle << "," << printPressureAngle << "," << dwTime << std::endl;
}

}
