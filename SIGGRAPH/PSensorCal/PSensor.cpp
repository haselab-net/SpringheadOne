#include "PSensor.h"
#include <stdio.h>
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
		printf("sensorValue = %f\n",sensor[i].sensorValue0);
	}
	for(int j=0; j<calibNum; j++){ //キャリブレイションの回数
		printf("How much is your weigh?\n");
		scanf("%f",&weigh);
		printf("%f\n",weigh);
		for(int i=0; i<number; i++){
			sensorValue = sensor[i].GetVol();
			printf("sensorValue = %f\n",sensorValue);
			printf("weigh = %f\n",weigh);
			sensor[i].numerator += weigh / number * (sensorValue - sensor[i].sensorValue0);
			sensor[i].denominator += (sensorValue - sensor[i].sensorValue0) * (sensorValue - sensor[i].sensorValue0);
			printf("sensor %d numerator = %f \n",i,sensor[i].numerator);
			printf("sensor %d denominator = %f \n",i,sensor[i].denominator);
		}
	}
	for(int i=0; i<number; i++){
		sensor[i].tilt = sensor[i].numerator / sensor[i].denominator;
	}
}

void PSensor::PrintFile(FILE *fp){
	for(int i=0; i<number; i++){
		float a = sensor[i].tilt;
		float b = sensor[i].sensorValue0;
		fprintf(fp,"%f %f\n",a,b);
	}
}
