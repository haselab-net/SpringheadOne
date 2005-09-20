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
	strcpy(filestr, "C:\\VSS\\PROJECT\\HumanScale\\vroadrunner\\HIS\\calibValue.txt");
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
	std::cout << "Set Center of Gravity" << std::endl;
	SetCenterOfGravity(centerOfGravity[0]);
	cog = centerOfGravity[0];
	//	5回前にサンプリングした値との比較をして、重心の変化の大きさをdCenterOfGravityとする
	float dCenterOfGravity = (centerOfGravity[0] - centerOfGravity[CENTER_OF_GRAVITY_LEN-1]).norm() / (0.02f*(CENTER_OF_GRAVITY_LEN-1));

	float alpha = 0.1f;
	cogAve = alpha * cog + (1-alpha) * cogAve;
	gravityDiff = cog - cogAve;
	walkingDirection = Matrix2f(Rad(90)) * gravityDiff;

	dwLastTime = dwTime;
	dwTime = GetTickCount();
	float dt = float(dwTime - dwLastTime) / 1000;

	float beta=0.1f;
	if (dCenterOfGravity < threshold){
		direction = Matrix2f(dt*turnVel) * direction;  // 重心移動が少ない時は、角度のみ変化
	} else if (direction * walkingDirection > 0){ 
		direction = beta*walkingDirection + (1-beta)*direction;
	} else {
		direction = -beta*walkingDirection + (1-beta)*direction;
	}

	inner = atan2(direction.Y(), direction.X());
	std::cout << "inner = " << inner/ 3.141592 * 180 << std::endl;

	float minRad = M_PI / 6;
	if (memoryOfInner > minRad && inner < 0)
		inner = inner + M_PI;
	if (memoryOfInner < -minRad && inner > 0)
		inner = inner - M_PI;
	memoryOfInner = inner;

	frontCheck = inner < Rad(frontGap);

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
	std::cout << "step = " << step << std::endl;
	std::cout << "BackStep = " << backStep << std::endl;
	std::cout << "SideStep = " << sideStep << std::endl;
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

}
