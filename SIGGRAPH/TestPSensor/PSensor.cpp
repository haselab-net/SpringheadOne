#include "PSensor.h"
#include <math.h>
#include <iostream>
#include <Base/Affine.h>
#include <Device/DRAdapio.h>
using namespace LVRM;
using namespace std;

PSensor::PSensor(int n){
	number = n;
	exist = CheckSensor();
}

bool PSensor::CheckSensor(){
    dev.RPool().Register(new DRAdapio(10));
	dev.Init();
	for(int i=0; i<number; i++){
		dev.VPool().Rent(sensor[i].ad);	
		if (!sensor[i].ad){
			printf("This Device Not Exist\n");
			return false;
		}
	}
	return true;
}

void PSensor::Calibration(int calibNum){
	printf("Weigh = 0\n");
	for(int i=0; i<number; i++){
		sensor[i].sensorValue0 = sensor[i].GetVol();
	}
	for(int j=0; j<calibNum; j++){ //キャリブレイションの回数
		printf("How much is your weigh?\n");
		scanf("%d",&weigh);
		for(int i=0; i<number; i++){
			sensorValue = sensor[i].GetVol();
			sensor[i].numerator += weigh / number * (sensorValue - sensor[i].sensorValue0);
			sensor[i].denominator += sensorValue - sensor[i].sensorValue0;
		}
	}
	for(int i=0; i<number; i++){
		sensor[i].tilt = sensor[i].numerator / sensor[i].denominator;
	}
}

void PSensor::ReadFile(FILE *fp){
	float a,b;
	for(int i=0; i<number; i++){
		fscanf(fp,"%f %f\n",&a,&b);
		sensor[i].tilt = a;
		sensor[i].sensorValue0 = b;
	}
}

float PSensor::TestWeigh(){
	float value = 0.0f;
	for(int i = 0; i < number; i++){
		value += sensor[i].tilt * (sensor[i].GetVol() - sensor[i].sensorValue0);
	}
	return value;
}
