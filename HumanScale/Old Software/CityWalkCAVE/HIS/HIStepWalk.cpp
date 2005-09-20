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
	pitchParam = 0.4f;
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
	threshold = 0.1f;
	memoryOfInner = 0.0f;
	directionMem = 0.0f;
	jumpPower = 0.0f;
	alpha = 0.1f;
	beta = 0.1f;
	isWalking = false;
	offset[0] = 1.0f;   ///�n�ʂ������̓^�[���e�[�u�����X���Ă���Ƃ��l��ω������Ă݂Ă͂ǂ����낤��
	offset[1] = 1.0f;
	offset[2] = 1.0f;
	offset[3] = 1.0f;
	eyeAngle = 0.0f;
	lastDCenterAngle = 0.0f; 
	lastDCenterG = Vec2f(0.0f, 0.0f);
	footSide = 0;
	footSideFlag = false;
	vecFooToFoot = Vec2f(0.0f, 0.0f);
	centerGNorm = 0.0f;
	firstVec[0] = Vec2f(0.0f, 0.0f);
	firstVec[1] = Vec2f(0.0f, 0.0f);
	thresholdFootHigh = 0.006f;
	thresholdFootLow = 0.002f;
	notUsing = 5000;
	sumMaxMinSetTime = 500;
	strcpy(filestr, "C:\\Home\\VSS\\PROJECT\\HumanScale\\CityWalk\\HIS\\calibValue.txt");
}

HIStepWalk::~HIStepWalk(){
}
/*
void HIStepWalk::Print(std::ostream& os) const{
	float voltageForP;
	os.precision(3);
	os.width(6);
//	os <<"step:" << step << "  vel:" << walkVel << " dCOG:" << dCenterOfGravity;
//	os << "sensor:";
/// �Z���T�̒l���A�E�g�v�b�g

	for(int i=0; i<NSENSOR; ++i){
		os << sensor[i].rawCur << " ";
	}


/// ���K����̃Z���T�̒l���A�E�g�v�b�g

	for(int i=0; i<NSENSOR; ++i){
		os << sensor[i].Get() << " ";
	}


/// �d�S�̈ʒu���A�E�g�v�b�g
	os << centerOfGravity[0].X() << "," << centerOfGravity[0].Y();

	for(int i = 0 ; i < NSENSOR ; i++ ){
		voltageForP = sensor[i].Get();
		os << "," << voltageForP;
	}

	os << std::endl;
}
*/
//	�Z���T�[�̈ʒu
Vec2f sensorPos[] = {
#ifndef SENSOR4
	Vec2f(0,-1.0f*0.4f),
	Vec2f(-0.866f*0.4f, 0.5f*0.4f),
	Vec2f(0.866f*0.4f, 0.5f*0.4f),	// (��3 / 2) = 0.86602540378443864676372317075294���
#else
	//4�Z���T�Ή��B�ʒu�͂Ă��Ƃ��B
	Vec2f(0.0f*0.4f, 1.0f*0.4f),
	Vec2f(1.0f*0.4f, 0.0f*0.4f),
	Vec2f(0.0f*0.4f, -1.0f*0.4f),
	Vec2f(-1.0f*0.4f, 0.0f*0.4f),
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

	//�L�����u���[�V�����̊���l��ǂ݂���
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

	bool back_flag;
	back_flag = false;

	/// ���̓Z���T�̃L�����u���C�V����
	if (pSenCal){
		PSenCal();
	}

	//	�d�S�ʒu�̗������X�V����
	for(int i = 0 ; i < CENTER_OF_GRAVITY_LEN-1 ; i++ ){
		centerOfGravity[i+1] = centerOfGravity[i];
	}
	//	�d�S�����߂�D
	SetCenterOfGravity(centerOfGravity[0]);
	cog = centerOfGravity[0];
	//	5��O�ɃT���v�����O�����l�Ƃ̔�r�����āA�d�S�̕ω��̑傫����dCenterOfGravity�Ƃ���
	float dCenterOfGravity = (centerOfGravity[0] - centerOfGravity[CENTER_OF_GRAVITY_LEN-1]).norm() / (0.02f*(CENTER_OF_GRAVITY_LEN-1));

	cogAve = alpha * cog + (1-alpha) * cogAve;
	gravityDiff = cog - cogAve;
	walkingDirection = Matrix2f(Rad(90)) * gravityDiff;  // player�̑̂̌���

	dwLastTime = dwTime;
	dwTime = GetTickCount();
	float dt = float(dwTime - dwLastTime) / 1000;

	if (dCenterOfGravity < threshold){
#ifndef SLC_USE	
		direction = Matrix2f(dt*turnVel) * direction;  // �d�S�ړ������Ȃ����́A�p�x�̂ݕω�
#endif
		isWalking = false;
	} else if (direction * walkingDirection > 0){ 
		direction = beta*walkingDirection + (1-beta)*direction;
		isWalking = true;
	} else {
		direction = -beta*walkingDirection + (1-beta)*direction;
		isWalking = true;
	}

	/*
	if ((cogAve-cog).norm() < threshold){
		newWayDirection = Matrix2f(dt*turnVel) * newWayDirection;  // �d�S�ړ������Ȃ����́A�p�x�̂ݕω�
	} else if (newWayDirection * walkingDirection > 0){ 
		if(fabs(atan2(walkingDirection.X(),walkingDirection.Y())-atan2(directionMem.X(),directionMem.Y())) < M_PI /18)
			newWayDirection = walkingDirection;
		else
		newWayDirection = beta*walkingDirection + (1-beta)*newWayDirection;
	directionMem = walkingDirection;
	} else {
		if(fabs(atan2(walkingDirection.X(),walkingDirection.Y())+atan2(directionMem.X(),directionMem.Y())) < M_PI /18)
			newWayDirection = -walkingDirection;
		else
			newWayDirection = -beta*walkingDirection + (1-beta)*newWayDirection;
	directionMem = -walkingDirection;
	}

	newWayInner = atan2(newWayDirection.X(),newWayDirection.Y());
	*/

	inner = atan2(direction.X(), direction.Y());   //Y���Ƃ̂Ȃ��p����邽��x,y�𔽑΂ɂ��Ă���.
//	std::cout << "inner = " << inner/ 3.141592 * 180 << std::endl;
/*
	newWayInner = atan2(walkingDirection.X(),walkingDirection.Y());
	if (newWayDirection * walkingDirection > 0){ 
		if( fabs(newWayInner-directionMem) < M_PI /18)
			directionMem = newWayInner;
		else
			d = beta*newWayInner + (1-beta)*Inner;
	} else {
		if( fabs(newWayInner+directionMem) < M_PI /18)
*/	


	// �t�����ɂȂ�Ȃ��悤�ɂ��邽�߂̐���
// �ڐ��Ń^�[���e�[�u�����������ꍇ�͕s�v
#ifndef SLC_USE	
	float minRad = M_PI / 6;
	if (memoryOfInner > minRad && inner < 0)
		inner = inner + M_PI;
	if (memoryOfInner < -minRad && inner > 0)
		inner = inner - M_PI;
#else
	if (fabs(inner - eyeAngle) > M_PI / 2){
		if(inner>=0)
			inner = inner - M_PI;
		else
			inner = inner + M_PI;
	}
#endif

	memoryOfInner = inner;

	frontCheck = fabs(inner) < Rad(frontGap);


//  �������m
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
//		walkVel = (float) (0.2 * pitch) * pitch; //���傤�ǂ悢���炢?
//		walkVel = (float) pitchParam * pitch;

		dwStepTime = dwStepTime + dwInterval;
	}else if(dwInterval * pitch > 1000){	//�P�����A��������
		walkVel = 0;						//���s���x���O�ɖ߂�
	}

	FootSide();///�ǂ���̑����^�[���e�[�u���ɂ��Ă��邩�𔻒f

	// �ڐ������Ń^�[���e�[�u�����񂷂Ƃ��͕s�v
	//��]���͑O�ɐi�܂Ȃ�
#ifndef SLC_USE	
	const float gap = 10;
	if(fabs(inner) > Rad(gap) && rotateSwitch){
		//walkvel = 0;
		walkVel = walkVel * M_PI / 180 / fabs(inner) * gap; // ��]�p�x�ɂ���]���x�𒲐�
	}
#endif

//	if(BackStep(0.1f)){
//		walkVel = -walkVel;
//		backStep++;
//	}

/*
	std::cout << "step = " << step << std::endl;
	std::cout << "backStep = " << backStep << std::endl;
	std::cout << "SideStep = " << sideStep << std::endl;
*/

//  �������m�i�����܂Łj


//	centerOfGravity[0] /= NSENSOR; //����Ȃ��H

/*  (�ȑO�̕��s���m�v���O����)
//	dCenterOfGravity *= 0.8f;
	dCenterOfGravity = (centerOfGravity[0] - centerOfGravity[CENTER_OF_GRAVITY_LEN-1]).norm() / (0.02f*(CENTER_OF_GRAVITY_LEN-1));
	dwInterval = timeGetTime() - dwStepTime;
	if (dCenterOfGravity > 0.5f && dwInterval >= 300){
		step++;
		//	���� = (0.2 * pitch + 0.5)	���_�̃f�[�^���Q�ƁBpitch = �P�b������̕���
		pitch = (float) 1000 / dwInterval;	
		walkVel = (float) (0.2 * pitch + 0.5) * pitch;
		dwStepTime = dwStepTime + dwInterval;
	}else if(dwInterval * pitch > 1000){	//�P�����A��������
		walkVel = 0;						//���s���x���O�ɖ߂�
	}
�i�����܂Łj
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
void HIStepWalk::PSenSetMin(){
	for(int i = 0 ; i < NSENSOR ; i++ ){
		sensor[i].SetMin();
	}
}

void HIStepWalk::PSenSetMax(){
	for(int i = 0 ; i < NSENSOR ; i++ ){
		sensor[i].SetMax();
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
		centerG += sensor[i].Get() * offset[i] * sensor[i].pos;
	}
}

void HIStepWalk::SetJump(float& jumpPower){
	jumpPower = 0.0f;
	for(int i = 0 ; i < NSENSOR ; i++ ){
		jumpPower += sensor[i].Get();
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

// SIGGRAPH �f�[�^�擾�p�֐�  

void HIStepWalk::DataOutput(float x, float y, float z) {
//	std::cout << "flag_SIGGRAPH =            " << flag_SIGGRAPH << std::endl;

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
			*dataFileSIGGRAPH << x << "," << z << "," << cog.X() << "," << cog.Y() << "," << inner << "," << inner2 << "," << step << "," << dwTime << std::endl;
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

bool HIStepWalk::BackStep(float value){
	Vec2f back_point;    //���̓_�����ɂ���ꍇ�o�b�N�X�e�b�v����
	Vec2f back_pointV;   //back_point����cog�ւ̃x�N�g��
	float back_point_angle;    //back_point�̊p�x
	back_point_angle = M_PI/2.0 - inner - M_PI;
	back_point = value*Vec2f(cos(back_point_angle),sin(back_point_angle));
	back_pointV = cogAve - back_point;
	if(back_pointV * direction < 0)
		return true;
	return false;
}

void HIStepWalk::Print(std::ostream& os) const{
	float voltageForP;
	os.precision(3);
	os.width(6);
//	os <<"step:" << step << "  vel:" << walkVel << " dCOG:" << dCenterOfGravity;
//	os << "sensor:";
/// �Z���T�̒l���A�E�g�v�b�g
/*
	for(int i=0; i<NSENSOR; ++i){
		os << sensor[i].rawCur << " ";
	}
*/

/// ���K����̃Z���T�̒l���A�E�g�v�b�g
/*
	for(int i=0; i<NSENSOR; ++i){
		os << sensor[i].Get() << " ";
	}
*/

/// �d�S�̈ʒu���A�E�g�v�b�g
	os << centerOfGravity[0].X() << "," << centerOfGravity[0].Y();

	for(int i = 0 ; i < NSENSOR ; i++ ){
		voltageForP = sensor[i].cur;
		os << "," << voltageForP;
	}

	os << "," << footSide << "," << nowDCenterG.X() << "," << nowDCenterG.Y() << "," << centerGNorm << "," << timeGetTime() << "," << vecFooToFoot.X() << "," << vecFooToFoot.Y() << "," << footSideFlag << "," << testFootSide[0] <<  "," << testFootSide[1] << "," << testFootSide[2] << std::endl;
}
//test

int HIStepWalk::FootSide(){
	nowDCenterG = Vec2f(0.0f, 0.0f);
	for(int i = 0 ; i < CENTER_OF_GRAVITY_LEN-1 ; i++ ){
		nowDCenterG = nowDCenterG + (centerOfGravity[i] - centerOfGravity[i+1]);
	}
	nowDCenterG = nowDCenterG / (CENTER_OF_GRAVITY_LEN - 1);
//	nowDCenterAngle = atan2(nowDCenterG.Y() - lastDCenterG.Y(), nowDCenterG.X() - lastDCenterG.X());
	nowDCenterAngle = atan2(lastDCenterG.Y() - nowDCenterG.Y(), lastDCenterG.X() - nowDCenterG.X());
	centerGNorm = sqrt(nowDCenterG.X() * nowDCenterG.X() + nowDCenterG.Y() * nowDCenterG.Y());
	if(centerGNorm <= thresholdFootLow && footSideFlag || (vecFooToFoot * nowDCenterG < 0) && centerGNorm <= thresholdFootHigh && footSideFlag){
		footSideFlag = false;
//		float fs = sin(nowDCenterAngle - lastDCenterAngle);
		float fooToFootAngle = atan2(vecFooToFoot.Y(), vecFooToFoot.X());
//		float fs = sin(nowDCenterAngle - lastDCenterAngle);
		float fs = 0.0f;
		if(lastDCenterG * nowDCenterG < 0){
			fs = sin(atan2(lastDCenterG.Y(),lastDCenterG.X()) - fooToFootAngle);
		}else{
			fs = sin(atan2(nowDCenterG.Y(),nowDCenterG.X()) - fooToFootAngle);
		}
		if(fs > 0){
			footSide = -1;
			testFootSide[0] = -1;
		}else if(fs < 0){
			footSide = 1;
			testFootSide[0] = 1;
		}
		else{
			footSide = 0;
		}
		fs = sin(nowDCenterAngle - fooToFootAngle);
		if(fs > 0){
			footSide--;
			testFootSide[1] = -1;
		}else if(fs < 0){
			footSide++;
			testFootSide[1] = 1;
		}
		else{
			footSide = 0;
		}
		fs = sin(atan2(firstVec[1].Y(),firstVec[1].X()) - fooToFootAngle);
		if(fs > 0){
			footSide++;
			testFootSide[2] = 1;
		}else if(fs < 0){
			footSide--;
			testFootSide[2] = -1;
		}
		else{
			footSide = 0;
		}
		vecFooToFoot = Vec2f(0.0f, 0.0f);
	}else if(centerGNorm > thresholdFootLow){
		vecFooToFoot += nowDCenterG;
		if(firstVec[0].X() == 0.0f && firstVec[0].Y() == 0.0f){
			firstVec[0] = nowDCenterG;
		}
//		footSide = 0;
	}
	if(centerGNorm > thresholdFootHigh){
		footSideFlag = true;
		footSide = 0;
		if(firstVec[1].X() == 0.0f && firstVec[1].Y() == 0.0f){
			firstVec[1] = firstVec[0];
		}
	}
	if(centerGNorm <= thresholdFootLow){
		firstVec[0] = Vec2f(0.0f, 0.0f);
		firstVec[1] = Vec2f(0.0f, 0.0f);
		vecFooToFoot = Vec2f(0.0f, 0.0f);
	}
	lastDCenterG = nowDCenterG;
	lastDCenterAngle = nowDCenterAngle;
	return footSide;
}

float HIStepWalk::SumSensor(){
	float sumSensorsValue = 0.0f;
	for(int i = 0 ; i < NSENSOR ; i++ ){
		sumSensorsValue += sensor[i].Get();
	}
	return sumSensorsValue;
}

int HIStepWalk::StatusOfUser(){
	for(int i = 0; i < 5; i++){
		sumSensors[i+1] = sumSensors[i];
	}
	sumSensors[0] = SumSensor();
	float sumTime = timeGetTime();
	if(sumSensors[0] > 0.2){  //���[�U������ĂȂ��Ƃ���r�����邽��
		sumSensorAve = 0.98 * sumSensorAve + 0.02 * sumSensors[0];  //�Z���T�̘a�̕��ς����
	}

	if(sumSensorAve * 1.1 < sumSensors[0] || (sumTime - sumSensorBig.time) > notUsing){
		sumSensorBig.value = sumSensors[0];
		sumSensorBig.time = sumTime;
	}
	else if(sumSensorAve * 0.9 > sumSensors[0] || (sumTime - sumSensorSmall.time) > notUsing){
		sumSensorSmall.value = sumSensors[0];
		sumSensorSmall.time = sumTime;
	}

	if(sumSensorBig.value < 0.1){
		return 0;
	}
	else if(fabs(sumSensorBig.value - sumSensorSmall.value) < 0.05){ ///�l�͓K��
		return 1;
	}

	if(sumSensorMax.value < sumSensors[0]){
		sumSensorMax.value = sumSensors[0];
		sumSensorMax.time = sumTime;
	}
	else if((sumTime - sumSensorMax.time) > sumMaxMinSetTime){
		sumSensorMax.value = sumSensorAve;
		sumSensorMax.time = sumTime;
	}

	if(sumSensorMin.value > sumSensors[0]){
		sumSensorMin.value = sumSensors[0];
		sumSensorMin.time = sumTime;
	}
	else if((sumTime - sumSensorMin.time) > sumMaxMinSetTime){
		sumSensorMin.value = sumSensorAve;
		sumSensorMin.time = sumTime;
	}
/// �������牺��statusOfUser�𒲂ׂ邽�߂̏�������
	if(sumSensors[0] < sumSensorAve * 0.6 && statusOfUser != 4 && statusOfUser != 9){
		if(sumSensorMin.value != sumSensorSmall.value){
			float sumSensorDef = 0.0f;
			for(int i = 0; i < 5; i++){
				sumSensorDef += sumSensors[i] - sumSensors[i+1];
			}
			if(fabs(sumSensorDef / 5.0f) < 0.05){
				if(statusOfUser == 8 && sumSensorMax.value > sumSensorAve * 1.1){
					statusOfUser = 9;
				}else{
					statusOfUser = 8;
					sumSensorMax.value = sumSensorAve;
					sumSensorMax.time = sumTime;
				}
			}
			else if(statusOfUser != 8){
				statusOfUser = 4;
			}
		}
	}
    else
	if(statusOfUser == 9 && sumSensors[0] > sumSensorAve * 0.6){
		statusOfUser = 1;
	}
    else
	if(statusOfUser != 7 && sumSensors[0] < sumSensorAve * 0.1 && sumSensors[1] < sumSensorAve * 0.1 && sumSensorMax.value > sumSensorAve * 1.3){
		statusOfUser = 7;
	}
	else if(statusOfUser == 7 && sumSensors[0] > sumSensorAve){
		statusOfUser = 1;
		sumSensorMax.value = sumSensorAve;
		sumSensorMax.time = sumTime;
	}

	return statusOfUser;
}

/*  //offset����Ƃ��Ɏg��.
	if( fabs(sumSensorAve - 1.0f) > 0.1 && statusOfUser == 1){
		for(int i = 0 ; i < NSENSOR ; i++ ){
			offset[i] = offset[i] / sumSensorAve;
		}
	}
*/

}

