#include "HIStepWalk.h"
#include <math.h>
#include <iostream>
#include <WinBasis/WBWin.h>
#include <Base/Affine.h>
#include <strstream>
#include <io.h>
#include <fstream>


namespace LVRM{;

HIStepWalk::HIStepWalk(){
	pitchParam = 5.0f;
	dwLastTime = 0.0f;
	dwTime = 0.0f;
	sideStep = 0;
	frontGap = 5.0f;
	step = 0;
	backStep = 0;
	walkVel = 0;
	dwStepTime = 0;
	pSenCal = false;
	printForCenterG = false;
	rotateSwitch = true;
	dataFileSIGGRAPH = NULL;
	flag_SIGGRAPH = false;
	threshold = 0.7f;
	memoryOfInner = 0.0f;
//	directionMem = 0.0f;
	jumpPower = 0.0f;
	alpha = 0.1f;
	beta = 0.3f;
	isWalking = false;
	offset[0] = 1.0f;   ///地面もしくはターンテーブルが傾いているとき値を変化させてみてはどうだろうか
	offset[1] = 1.0f;
	offset[2] = 1.0f;
	offset[3] = 1.0f;
//	eyeAngle = 0.0f;
	lastDCenterAngle = 0.0f; 
	lastDCenterG = Vec2f(0.0f, 0.0f);
	checkSideDirection = false;
	thresholdFootHigh = 0.1f;
	thresholdFootLow = 0.03f;
	notUsing = 5000;
	sumMaxMinSetTime = 2000;
	sumSensorAve = 1.0f;
	sumTime = 0;///下のLogChangeMin,Max用
	for(int i = 0; i < 2; i++){//sumSensorMin,Maxの初期化
		LogChangeMin();
		LogChangeMax();
	}
	syagamiFlag = false;
	sideStepV.time = 0;
	checkNum = 0;
	footToFoot = Vec2f(0.0f, 0.0f);
	strcpy(filestr, "C:\\Home\\VSS\\PROJECT\\HumanScale\\vroadrunner\\HIS\\calibValue.txt");
}
HIStepWalk::~HIStepWalk(){
}
void HIStepWalk::Print(std::ostream& os) const{
	os.precision(3);
	os.width(6);
//	os <<"step:" << step << "  vel:" << walkVel << " dCOG:" << dCenterOfGravity;
//	os << "sensor:";
/// センサの値をアウトプット
/*
	for(int i=0; i<NSENSOR; ++i){
		os << sensor[i].rawCur << " ";
	}
*/

/// 正規化後のセンサの値をアウトプット
/*
	for(int i=0; i<NSENSOR; ++i){
		os << sensor[i].Get() << " ";
	}
*/

/// 重心の位置をアウトプット
	os << centerOfGravity[0].X() << "," << centerOfGravity[0].Y();

	os << std::endl;
}

//	センサーの位置
Vec2f sensorPos[] = {
#ifndef SENSOR4
	Vec2f(1*0.4f, 0),
	Vec2f(-0.5f*0.4f,  0.866f*0.4f),
	Vec2f(-0.5f*0.4f, -0.866f*0.4f),	// (√3 / 2) = 0.86602540378443864676372317075294･･･
#else
	//4センサ対応。位置はてきとう。
//	Vec2f(0.0f*0.4f, 1.0f*0.4f),
//	Vec2f(1.0f*0.4f, 0.0f*0.4f),
//	Vec2f(0.0f*0.4f, -1.0f*0.4f),
//	Vec2f(-1.0f*0.4f, 0.0f*0.4f),
	// (√2 / 2) = 0.70710678
	Vec2f( 0.70710678f,  0.70710678f),
	Vec2f( 0.70710678f, -0.70710678f),
	Vec2f(-0.70710678f, -0.70710678f),
	Vec2f(-0.70710678f,  0.70710678f),
#endif
};
bool HIStepWalk::Init(DVDeviceManager& dev){
	for(int i=0; i<NSENSOR; ++i){
		dev.VPool().Rent(sensor[i].ad);	
		if (!sensor[i].ad) return false;
		sensor[i].pos = sensorPos[i];
	}
	for(int i=0; i<NSENSOR; ++i){
		sensor[i].CalibMin();
	}
	dwStepTime = GetTickCount();
	dwTime = dwStepTime;

	//キャリブレーションの既定値を読みこみ
	char buffer[BUFFER];
	if((calibValue = fopen(filestr, "r")) == NULL){
		std::cout << "cannot open file" << std::endl;
		exit(0);
	}
	for(int i = 0; i < NSENSOR; i++){
		fgets(buffer, 256, calibValue);
		sensor[i].offsetMax = atof(buffer);
		std::cout << "sensor[" << i << "].Max = " << sensor[i].offsetMax << std::endl;
		fgets(buffer, 256, calibValue);
		sensor[i].offsetMin = atof(buffer);
		std::cout << "sensor[" << i << "].Min = " << sensor[i].offsetMin << std::endl;
	}
	fclose(calibValue);
	calibValue = NULL;
	PSenRange();

	return true;
}
void HIStepWalk::Step(float turnVel){

	/// 圧力センサのキャリブレイション
	if (pSenCal){
		PSenCal();
	}

	//	重心位置の履歴を更新する
	for(int i = 0 ; i < CENTER_OF_GRAVITY_LEN-1 ; i++ ){
		centerOfGravity[i+1] = centerOfGravity[i];
	}
	//	重心を求める．
//	std::cout << "Set Center of Gravity" << std::endl;
	SetCenterOfGravity(centerOfGravity[0]);
	cog = centerOfGravity[0];
	//	5回前にサンプリングした値との比較をして、重心の変化の大きさをdCenterOfGravityとする
	dCenterOfGravity = (centerOfGravity[0] - centerOfGravity[CENTER_OF_GRAVITY_LEN-1]).norm() / (0.02f*(CENTER_OF_GRAVITY_LEN-1));

	alpha = 0.1f;
	cogAve = alpha * cog + (1-alpha) * cogAve;
	gravityDiff = cog - cogAve;
	walkingDirection = Matrix2f(Rad(90)) * gravityDiff;

	dwLastTime = dwTime;
	dwTime = GetTickCount();
	dt = (dwTime - dwLastTime) / 1000.0f;

	beta=0.1f;
	if (dCenterOfGravity < threshold){
		direction = Matrix2f(dt*turnVel) * direction;  // 重心移動が少ない時は、角度のみ変化
	} else if (direction * walkingDirection > 0){ 
		direction = beta*walkingDirection + (1-beta)*direction;
	} else {
		direction = -beta*walkingDirection + (1-beta)*direction;
	}

	std::cout << StatusOfUser() << std::endl;

	inner = atan2(direction.Y(), direction.X());
//	std::cout << "inner = " << inner/ 3.141592 * 180 << std::endl;

	float minRad = M_PI / 6;
	if (memoryOfInner > minRad && inner < 0)
		inner = inner + M_PI;
	if (memoryOfInner < -minRad && inner > 0)
		inner = inner - M_PI;
	memoryOfInner = inner;

	frontCheck = inner < Rad(frontGap);
/*
//  歩数検知
	for (int i = 0; i < 2; i++)
		resultOfInner[i+1] = resultOfInner[i];
	resultOfInner[0] = direction * walkingDirection;

	dwInterval = timeGetTime() - dwStepTime;
	if (resultOfInner[0] * resultOfInner[1] <= 0 && dCenterOfGravity > threshold){   
		if (SideStep() && frontCheck){
			sideStep++;
		}
		step++;
		pitch = (float) 1000 / dwInterval;	
//		walkVel = (float) (0.2 * pitch + 0.5) * pitch;
		walkVel = (float) pitchParam * (0.2 * pitch + 0.5) * pitch;
//		walkVel = (float) (0.2 * pitch) * pitch; //ちょうどよいくらい?
//		walkVel = (float) pitchParam * pitch;

		dwStepTime = dwStepTime + dwInterval;
	}else if(dwInterval * pitch > 1000){	//１歩分、歩いたら
		walkVel = 0;						//歩行速度を０に戻す
	}
*/
	nowDCenterG = (centerOfGravity[0] - centerOfGravity[1]) / (dt);
	nowDCenterAngle = atan2(nowDCenterG.Y(),nowDCenterG.X());
	Vec2f testDirection = CheckDirection();
	lastDCenterG = nowDCenterG;
	lastDCenterAngle = nowDCenterAngle;

	//回転中は前に進まない
	const float gap = 10;
	if(fabs(inner) > Rad(gap) && rotateSwitch){
		//walkvel = 0;
		walkVel = walkVel * M_PI / 180 / fabs(inner) * gap; // 回転角度により回転速度を調整
	}

//	if(BackStep(0.2f)){
//		walkVel = -walkVel;
//		backStep++;
//	}
//	std::cout << "step = " << step << std::endl;
//	std::cout << "BackStep = " << backStep << std::endl;
//	std::cout << "SideStep = " << sideStep << std::endl;
//  歩数検知（ここまで）


//	centerOfGravity[0] /= NSENSOR; //いらない？

/*  (以前の歩行検知プログラム)
//	dCenterOfGravity *= 0.8f;
	dCenterOfGravity = (centerOfGravity[0] - centerOfGravity[CENTER_OF_GRAVITY_LEN-1]).norm() / (0.02f*(CENTER_OF_GRAVITY_LEN-1));
	dwInterval = timeGetTime() - dwStepTime;
	if (dCenterOfGravity > 0.5f && dwInterval >= 300){
		step++;
		//	歩幅 = (0.2 * pitch + 0.5)	卒論のデータを参照。pitch = １秒当たりの歩数
		pitch = (float) 1000 / dwInterval;	
		walkVel = (float) (0.2 * pitch + 0.5) * pitch;
		dwStepTime = dwStepTime + dwInterval;
	}else if(dwInterval * pitch > 1000){	//１歩分、歩いたら
		walkVel = 0;						//歩行速度を０に戻す
	}
（ここまで）
*/
}

void HIStepWalk::beforeCalib(){
	for(int i = 0; i < NSENSOR; i++){
		sensor[i].offsetMax = 0.0f;
		sensor[i].offsetMin = 10.0f;
	}
}

void HIStepWalk::PSenMin(){
	for(int i = 0 ; i < NSENSOR ; i++ ){
		sensor[i].CalibMin();
	}
}

void HIStepWalk::PSenMax(){
	for(int i = 0 ; i < NSENSOR ; i++ ){
		sensor[i].CalibMax();
	}
}

void HIStepWalk::SetMinVol() {
	for(int i = 0; i < NSENSOR; i++){
		sensor[i].SetMin();
	}
}

void HIStepWalk::PSenRange(){
	for(int i = 0 ; i < NSENSOR ; i++ ){
		sensor[i].scale = sensor[i].offsetMax - sensor[i].offsetMin;
	}
}

void HIStepWalk::ValueOutput(){
	if((calibOutput = fopen(filestr, "w")) == NULL){
		std::cout << "cannot open file to write" << std::endl;
		exit(0);
	}
	for(int i = 0; i < NSENSOR; i++){
		fprintf(calibOutput, "%f\n", sensor[i].offsetMax);
		fprintf(calibOutput, "%f\n", sensor[i].offsetMin);
		std::cout << "sensor[" << i << "].Max = " << sensor[i].offsetMax << std::endl;
		std::cout << "sensor[" << i << "].Min = " << sensor[i].offsetMin << std::endl;
	}
	fclose(calibOutput);
	calibOutput = NULL;
}

void HIStepWalk::SetCenterOfGravity(Vec2f& centerG){
	centerG = Vec2f();
	for(int i = 0 ; i < NSENSOR ; i++ ){
		centerG += sensor[i].Get() * sensor[i].pos;
	}
}

void HIStepWalk::PSenCal(){
	PSenMin();
	PSenMax();
}

bool HIStepWalk::SideStep(){
	if ( M_PI/2 < acos(direction * gravityDiff / (direction.norm() * gravityDiff.norm())) )
		return true;
	else
		return false;
}

bool HIStepWalk::BackStep(float value){
	Vec2f back_point;    //この点より後ろにいる場合バックステップする
	Vec2f back_pointV;   //back_pointからcogへのベクトル
	float back_point_angle;    //back_pointの角度
	back_point_angle = M_PI/2.0 - inner - M_PI;
	back_point = value*Vec2f(cos(back_point_angle),sin(back_point_angle));
	back_pointV = cogAve - back_point;
	if(back_pointV * direction < 0)
		return true;
	return false;
}

// SIGGRAPH データ取得用関数  

void HIStepWalk::DataOutput(float x, float y, float z) {
	std::cout << "flag_SIGGRAPH =            " << flag_SIGGRAPH << std::endl;

	if(x < -0.6 && x > -1.5 && z < 4.5 && z > 3.1 && y > 15 && y < 18){
		flag_SIGGRAPH = true;
	}
	if(flag_SIGGRAPH){
		if(dataFileSIGGRAPH == NULL && y > 15 && y < 20){
			if(3.6 < x && x < 4.3 && 0.3 < z && z < 1.0){
				folderName = "pointB/";
				strcpy( folderNameBack,folderName );
				fileNameTime = timeGetTime();			
				dataFileSIGGRAPH = new std::ofstream(strcat(folderNameBack,_itoa(fileNameTime,fileTime,10)));
				std::cout << "Now Getting Turn Data..." << std::endl;
			}else if(-1.7f > x && x > -2.4 && 3.6 < z && z < 4.4) {
				folderName = "pointA/";
				strcpy( folderNameBack,folderName );
				fileNameTime = timeGetTime();			
				dataFileSIGGRAPH = new std::ofstream(strcat(folderNameBack,_itoa(fileNameTime,fileTime,10)));
				std::cout << "Now Getting Turn Data..." << std::endl;
			}
		}else if(y > 15 && y < 20){
			std::cout << "Now Getting Turn Data..." << std::endl;
			float inner2 = atan2(walkingDirection.Y(), walkingDirection.X());
			*dataFileSIGGRAPH << x << "," << z << "," << cog.X() << "," << cog.Y() << "," << -inner << "," << inner2 << "," << step << "," << dwTime << std::endl;
		}
		if(y < 15){
			if (dataFileSIGGRAPH){
				delete dataFileSIGGRAPH;
				std::cout << "Stop Getting Turn Data..." << std::endl;
				dataFileSIGGRAPH = NULL;
				flag_SIGGRAPH = false;
			}
		}
	}
}

void HIStepWalk::CloseSIGGRAPHFile(){
	if (dataFileSIGGRAPH){
		delete dataFileSIGGRAPH;
		std::cout << "Stop Getting Turn Data..." << std::endl;
		dataFileSIGGRAPH = NULL;
		flag_SIGGRAPH = false;
	}
}


Vec2f HIStepWalk::CheckDirection(){
	if((dCenterOfGravity <= thresholdFootLow && checkSideDirection) || ((footToFoot * nowDCenterG < 0) && dCenterOfGravity <= thresholdFootHigh && checkSideDirection)){
		checkSideDirection = false;
//		float fooToFootAngle = atan2(vecFooToFoot.Y(), vecFooToFoot.X());
		for(int i = 0; i < checkNum-1; i++){
			if(sin(checkDir[10] - checkDir[i]) > 0){
				checkSide++;
			}else{
				checkSide--;
			}
		}
		if(checkSide == checkNum-1){
			userDir = Matrix2f(Rad(90)) * footToFoot;
		}else if(checkSide == -checkNum+1){
			userDir = -Matrix2f(Rad(90)) * footToFoot;
		}else{
			userDir = Vec2f(0.0f, 0.0f);
		}
		footToFoot = Vec2f(0.0f, 0.0f);
		checkSide = 0;
		checkNum = 0;
		if(direction * userDir < 0){
			std::cout << "---------------Bad!--------------" << std::endl;
			badNum++;
		}else if(direction * userDir > 0){
			std::cout << "--------------Good!--------------" << std::endl;
			badNum = 0;
		}else{
			std::cout << "--------------Can't Check!--------------" << std::endl;
		}
		if(badNum >= 2){
			badNum = 0;
			direction = -1 * direction;
		}
		return userDir;
	}
	else if(dCenterOfGravity > thresholdFootLow){
//		vecFooToFoot += nowDCenterG;
		footToFoot += nowDCenterG;
/*
		if(checkNum < 9){
			checkNum++;
			float checkAngle = sin(nowDCenterAngle - lastDCenterAngle);
			if(checkAngle < 0){
				checkSide++;
			}else if(checkAngle > 0){
				checkSide--;
			}
		}
*/
		if(checkNum < 10){
			checkDir[checkNum] = atan2(footToFoot.Y(), footToFoot.X());
			checkNum++;
		}
		checkDir[10] = atan2(footToFoot.Y(), footToFoot.X());
	}
	if(dCenterOfGravity > thresholdFootHigh){
		checkSideDirection = true;
	}
	if(dCenterOfGravity <= thresholdFootLow){
//		vecFooToFoot = Vec2f(0.0f, 0.0f);
		footToFoot = Vec2f(0.0f, 0.0f);
		checkSide = 0;
		checkNum = 0;
	}
	return Vec2f(0.0f,0.0f);
}


float HIStepWalk::SumSensor(){
	float sumSensorsValue = 0.0f;
	for(int i = 0 ; i < NSENSOR ; i++ ){
		sumSensorsValue += sensor[i].Get();
	}
	return sumSensorsValue;
}

void HIStepWalk::LogChangeMax(){
	sumSensorMax[1].value = sumSensorMax[0].value;
	sumSensorMax[1].dCenterG = sumSensorMax[0].dCenterG;
	sumSensorMax[1].time = sumSensorMax[0].time;
	sumSensorMax[0].value = 1.0f;
	sumSensorMax[0].dCenterG = dCenterOfGravity;
	sumSensorMax[0].time = sumTime;
}

void HIStepWalk::LogChangeMin(){
	sumSensorMin[1].value = sumSensorMin[0].value;
	sumSensorMin[1].dCenterG = sumSensorMin[0].dCenterG;
	sumSensorMin[1].time = sumSensorMin[0].time;
	sumSensorMin[0].value = 1.0f;
	sumSensorMin[0].dCenterG = dCenterOfGravity;
	sumSensorMin[0].time = sumTime;
}

void HIStepWalk::LogChangeDCenterGMax(){
	dCenterGMax[1].value = dCenterGMax[0].value;
	dCenterGMax[1].sumSensorValue[0] = dCenterGMax[0].sumSensorValue[0];
	dCenterGMax[1].sumSensorValue[1] = dCenterGMax[0].sumSensorValue[1];
	dCenterGMax[1].cog = dCenterGMax[0].cog;
	dCenterGMax[1].time = dCenterGMax[0].time;
	dCenterGMax[0].value = 0.0f;
	dCenterGMax[0].sumSensorValue[0] = sumSensorAve;
	dCenterGMax[0].sumSensorValue[1] = sumSensorAve;
	dCenterGMax[0].cog = cog;
	dCenterGMax[0].time = sumTime;
}

float HIStepWalk::GetSideStepDirection(){ 
	if(centerOfGravity[0].X() != 0.0f){
		return atan2(centerOfGravity[0].Y(),centerOfGravity[0].X());
	}
	else if(centerOfGravity[0].Y()>0){
		return M_PI / 2;
	}
	else{
		return -M_PI / 2;
	}
}

int HIStepWalk::StatusOfUser(){
//--------------圧力センサの和の変化を調べる-----------------//
	for(int i = 0; i < 5; i++){
		sumSensors[i+1] = sumSensors[i];
	}
	sumSensors[0] = SumSensor();
//	std::cout << sumSensors[0] << std::endl;
	sumTime = timeGetTime();
	if(sumSensors[0] > 0.2){  //ユーザが乗ってないときを排除するため
		sumSensorAve = 0.98f * sumSensorAve + 0.02f * sumSensors[0];  //センサの和の平均を取る
	}

	if(sumSensorMaxFlag){
		if(sumSensorMax[0].value < sumSensors[0] / sumSensorAve){
			sumSensorMax[0].value = sumSensors[0] / sumSensorAve;
			sumSensorMax[0].dCenterG = dCenterOfGravity;
			sumSensorMax[0].time = sumTime;
		}
		else if(sumSensorAve * 1.1 > sumSensors[0]){
			sumSensorMaxFlag = false;
			LogChangeMax();
		}
	}else if(sumSensorAve * 1.1 <= sumSensors[0]){
		sumSensorMaxFlag = true;
		sumSensorMax[0].value = sumSensors[0] / sumSensorAve;
		sumSensorMax[0].dCenterG = dCenterOfGravity;
		sumSensorMax[0].time = sumTime;
	}else if((sumTime - sumSensorMax[0].time) > sumMaxMinSetTime){
		sumSensorMaxFlag = false;
		LogChangeMax();
	}

	if(sumSensorMinFlag){
		if(sumSensorMin[0].value > sumSensors[0] / sumSensorAve){
			sumSensorMin[0].value = sumSensors[0] / sumSensorAve;
			sumSensorMin[0].dCenterG = dCenterOfGravity;
			sumSensorMin[0].time = sumTime;
		}
		else if(sumSensorAve * 0.9 < sumSensors[0]){
			sumSensorMinFlag = false;
			LogChangeMin();
		}
	}else if(sumSensorAve * 0.9 >= sumSensors[0]){
		sumSensorMinFlag = true;
		sumSensorMin[0].value = sumSensors[0] / sumSensorAve;
		sumSensorMin[0].dCenterG = dCenterOfGravity;
		sumSensorMin[0].time = sumTime;
	}else if((sumTime - sumSensorMin[0].time) > sumMaxMinSetTime){
		sumSensorMinFlag = false;
		LogChangeMin();
	}


	if(sumSensorAve * 1.1 < sumSensors[0] || (sumTime - sumSensorBig.time) > notUsing){
		sumSensorBig.value = sumSensors[0] / sumSensorAve;
		sumSensorBig.time = sumTime;
	}
	else if(sumSensorAve * 0.9 > sumSensors[0] || (sumTime - sumSensorSmall.time) > notUsing){
		sumSensorSmall.value = sumSensors[0] / sumSensorAve;
		sumSensorSmall.time = sumTime;
	}
//-------------------ここまでセンサーの和の変化-------------------------//


//-------------------荷重中心の変化とそのときのセンサーの和の値------------------------//
	if(dCenterGMaxFlag){
		if(dCenterGMax[0].value < dCenterOfGravity){
			dCenterGMax[0].value = dCenterOfGravity;
			dCenterGMax[0].sumSensorValue[0] = sumSensors[0] / sumSensorAve;
			dCenterGMax[0].sumSensorValue[1] = sumSensors[1] / sumSensorAve;
			dCenterGMax[0].cog = cog;
			dCenterGMax[0].time = sumTime;
		}else if(dCenterOfGravity < 0.15){
			dCenterGMaxFlag = false;
			LogChangeDCenterGMax();
		}
	}else if(dCenterOfGravity >= 0.15){
		dCenterGMaxFlag = true;
		dCenterGMax[0].value = dCenterOfGravity;
		dCenterGMax[0].sumSensorValue[0] = sumSensors[0] / sumSensorAve;
		dCenterGMax[0].sumSensorValue[1] = sumSensors[1] / sumSensorAve;
		dCenterGMax[0].cog = cog;
		dCenterGMax[0].time = sumTime;
	}else if((sumTime - dCenterGMax[0].time) > sumMaxMinSetTime){
		dCenterGMaxFlag = false;
		LogChangeDCenterGMax();
	}
//-------------------ここまで荷重中心の変化------------------------//

//--------------歩行検出-------------------//
	for (int i = 0; i < 2; i++)
		resultOfInner[i+1] = resultOfInner[i];
	resultOfInner[0] = direction * walkingDirection;

	dwInterval = sumTime - dwStepTime;
	if (resultOfInner[0] * resultOfInner[1] <= 0 && dCenterOfGravity > threshold && statusOfUser < 8){   
		pitch = (float) 1000 / dwInterval;	
//		walkVel = (float) (0.2 * pitch + 0.5) * pitch;
		walkVel = (float) pitchParam * (0.2 * pitch + 0.5) * pitch;
//		walkVel = (float) pitchParam * (0.2 * pitch + 0.5) * pitch * 10;
//		walkVel = (float) (0.2 * pitch) * pitch; //ちょうどよいくらい?
//		walkVel = (float) pitchParam * pitch;


		dwStepTime = dwStepTime + dwInterval;

		std::cout << sumSensors[0]/sumSensorAve << " ------- " << sumSensors[1]/sumSensorAve << std::endl;

		if (dCenterOfGravity > 0.3){
			moving = true;
		}
/* 没かな^^;
//--------------サイドステップ検出-------------------//
		if(dCenterOfGravity > 0.5){
			if(statusOfUser == 1 && sideStepV.sideStepVec.X() == 0.0 && sideStepV.sideStepVec.Y() == 0.0){
				;//何もしない
			}
			else if(gravityDiff * sideStepV.sideStepVec > 0){
//				statusOfUser = 5;//又は6　左右の判断今のところ必要なし
			std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			}else if(statusOfUser != 5){
				statusOfUser = 2;
			}
			std::cout << "---------------------------------------------------------" << std::endl;
			sideStepV.sideStepVec = gravityDiff;
			sideStepV.time = sumTime;
//			return statusOfUser;
		}
//--------------ここまでサイドステップ検出-------------------//
*/
/*		if(BackStep(1000.0f)){
			walkVel = -walkVel;
			if(statusOfUser != 8){
				statusOfUser = 4;
			}
		}else 
*/		if(statusOfUser != 8){//pitchやdCenterOfGravityから歩くと走るの区別をつけてもいいかも//「歩く」と「走る」の区別の必要性の無さから現在は走っていても「歩く」と判定する．
			statusOfUser = 2;
		}
//		return statusOfUser;

	}else if(dwInterval * pitch > 1000 && statusOfUser != 7){	//１歩分、歩いたら
		walkVel = 0;//歩行速度を０に戻す
		if(moving){
			moving = false;
		}
		if( 1 < statusOfUser && statusOfUser < 5){
			statusOfUser = 1;
		}
	}

	if(sumTime - sideStepStartTime > 1000){
		sideStepV.sideStepVec =Vec2f(0.0f,0.0f);
		sideStepV.time = sumTime;
		if(statusOfUser == 5 || statusOfUser == 6){
			walkVel = 0;
			statusOfUser = 1;
		}
	}
//-------------------ここまで歩行検出-------------------------//

	if(sumSensorBig.value < 0.3){
		statusOfUser = 0;
		walkVel = 0;
		return statusOfUser;
	}
	else if(fabs(sumSensorBig.value - sumSensorSmall.value) < 0.0001 && statusOfUser != 8){ ///値は適当
		sumSensorAve = sumSensors[0];
		statusOfUser = 1; 
		walkVel = 0;
		return statusOfUser;
	}

/// ここから下はstatusOfUserを調べるための条件分岐
	dSumSensors = sumSensors[0] - sumSensors[1];
	if(dSumSensors <= 0 && dLastSumSensors >= 0 && sumSensors[1] / sumSensorAve > 1.2 && statusOfUser != 10){
		if(sumSensorMax[0].value > 1.2 && sumSensorMin[1].value < 0.8 && (sumSensorMax[0].value + sumSensorMin[1].value) / 2 > 0.9 && (sumTime - sumSensorMin[1].time) < 750){
			if(sumSensorMax[0].dCenterG < 0.15){
				statusOfUser = 8;
			}else{
				statusOfUser = 3;
			}
		}
	}else if(statusOfUser == 8 && sumSensors[0] < sumSensorAve * 0.8){
		statusOfUser = 9;
	}
/*	if(sumSensors[0] < sumSensorAve * 0.6 && !moving && statusOfUser != 9){
		if(sumSensorMin[0].value != sumSensorSmall.value){
			sumSensorDef = (sumSensors[0] - sumSensors[5])/(dt*5);
			if(fabs(sumSensorDef) < 2.5){
				if(statusOfUser == 8 && syagamiFlag){
					statusOfUser = 9;
					syagamiFlag = false;
				}else{
					statusOfUser = 8;
					LogChangeMax();
				}
			}
			else if(statusOfUser != 8){
				statusOfUser = 3;
			}
		}
	}
	else 
	if(statusOfUser == 8 && sumSensors[0] > sumSensorAve){
		syagamiFlag = true;
	}
*/  else
	if(statusOfUser == 9 && sumSensors[0] > sumSensorAve * 0.9){
		walkVel = 0;
		statusOfUser = 1;
	}
    else
	if(statusOfUser != 7 && sumSensors[0] < sumSensorAve * 0.1 && sumSensors[1] < sumSensorAve * 0.1 && sumSensorMax[1].value > 1.3){
		statusOfUser = 7;
	}
	else if(statusOfUser == 7 && sumSensors[0] > sumSensorAve){
		walkVel = 0;
		statusOfUser = 10;
//		LogChangeMax();
	}
	else if(statusOfUser == 10 && sumSensors[0] < sumSensorAve * 1.05){
		statusOfUser = 1;
		LogChangeMax();
	}

//--------------サイドステップ検出-------------------//
	float sideStepPoint = 0.17f;
	if(centerOfGravity[0].norm() > sideStepPoint && dCenterOfGravity < 0.01){
		if(!sideStepFlag){
			sideStepFlag = true;
			sideStepStartTime = sumTime;
		}
		walkVel = (float) (sumTime - sideStepStartTime) * (sumTime - sideStepStartTime) / 10000 * (centerOfGravity[0].norm() - sideStepPoint + 0.1);
		statusOfUser = 5;
	}else{
		if(statusOfUser == 5){
			walkVel = 0;
			statusOfUser = 1;
		}
		sideStepFlag = false;
	}
//--------------ここまでサイドステップ検出-------------------//

	dLastSumSensors = dSumSensors;
	return statusOfUser;
}

/*  //offsetするときに使う.
	if( fabs(sumSensorAve - 1.0f) > 0.1 && statusOfUser == 1){
		for(int i = 0 ; i < NSENSOR ; i++ ){
			offset[i] = offset[i] / sumSensorAve;
		}
	}
*/

}

