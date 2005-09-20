#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "atlantis.h"

int count[NUM_SHARKS];

int FindShark(void){
	int i,lmin;
	float r,rmin;
	rmin=10000000000.;
	for(i=0;i<NUM_SHARKS;i++){
		r=sqrt((P[0]-sharks[i].x)*(P[0]-sharks[i].x)+(P[1]-sharks[i].y)*(P[1]-sharks[i].y)
			+(P[2]-sharks[i].z)*(P[2]-sharks[i].z));
		if(r<rmin){
			rmin=r;
			lmin=i;
		}
	}
	return lmin;
}
void MoveShark(int i){
	float X,Y,Z,ttheta,tpsi,thetal;

	sharks[i].v=2.0;
	X=P[0]-sharks[i].x;
	Y=P[1]-sharks[i].y;
	Z=P[2]-sharks[i].z;
	thetal=sharks[i].theta;

	ttheta = RAD * atan(Z/(sqrt(X*X+Y*Y)));
	tpsi = RAD * atan2(Y,X);
	sharks[i].phi = 0.0;

	if(sharks[i].theta-ttheta>20.0){
		sharks[i].theta-=10.0;
	}
	else if(sharks[i].theta-ttheta<-20.0){
		sharks[i].theta+=10.0;
	}
	else{
	sharks[i].theta=ttheta;
	}
	sharks[i].dtheta=sharks[i].theta-thetal;
	
	if(sharks[i].psi-tpsi>20.0){
		sharks[i].psi-=10.0;
		if (sharks[i].psi < -180.0) {
				sharks[i].psi += 360.0;
			}
	}
	else if(sharks[i].psi-tpsi<-20.0){
		sharks[i].psi+=10.0;
		if (sharks[i].psi > 180.0) {
			sharks[i].psi -= 360.0;
		}
	}
	else{
		sharks[i].psi=tpsi;
	}

	sharks[i].x += SHARKSPEED * sharks[i].v * cos(sharks[i].psi / RAD) * cos(sharks[i].theta / RAD);
	sharks[i].y += SHARKSPEED * sharks[i].v * sin(sharks[i].psi / RAD) * cos(sharks[i].theta / RAD);
	sharks[i].z += SHARKSPEED * sharks[i].v * sin(sharks[i].theta / RAD);

	if(sharks[i].x<10200.0){
		count[i]=0;
		shark_moving[i]=2;
	}
}

void SharkMiss2(int i){
	int j;
	float d[3],l,li,lj;
	for(j=0;j<NUM_SHARKS;j++){
		if(j!=i){
			d[0] = sharks[j].x - sharks[i].x;
			d[1] = sharks[j].y - sharks[i].y;
			d[2] = sharks[j].z - sharks[i].z;
			l=sqrt(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);

			if(l<SHARKSIZE){
				li=sqrt((sharks[i].x-P[0])*(sharks[i].x-P[0])
						+(sharks[i].y-P[1])*(sharks[i].y-P[1])
						+(sharks[i].z-P[2])*(sharks[i].z-P[2]));
				lj=sqrt((sharks[j].x-P[0])*(sharks[j].x-P[0])
						+(sharks[j].y-P[1])*(sharks[j].y-P[1])
						+(sharks[j].z-P[2])*(sharks[j].z-P[2]));
				if(li>lj) sharks[i].v=0.0;
				else sharks[j].v=0.0;
			}
		}
	}
}

void ReturnShark(int i){
	int j;
	float X,Y,Z,ttheta,tpsi,thetal;
	sharks[i].v=2.0;

	if(count[i]<30){
		sharks[i].x += 500.0;
		sharks[i].y += SHARKSPEED * sharks[i].v * sin(sharks[i].psi / RAD) * cos(sharks[i].theta / RAD);
		sharks[i].z += SHARKSPEED * sharks[i].v * sin(sharks[i].theta / RAD);
		if(sharks[i].theta>0.0&&sharks[i].theta<90.0) sharks[i].theta+=1.0;
		else sharks[i].theta-=1.0;
		count[i]++;
	}
	else {
		X=sharks_org[i].x-sharks[i].x;
		Y=sharks_org[i].y-sharks[i].y;
		Z=sharks_org[i].z-sharks[i].z;
		thetal=sharks[i].theta;

		ttheta = RAD * atan(Z/(sqrt(X*X+Y*Y)));
		tpsi = RAD * atan2(Y,X);
		sharks[i].phi = 0.0;

		if(sharks[i].theta-ttheta>20.0){
			sharks[i].theta-=10.0;
		}
		else if(sharks[i].theta-ttheta<-20.0){
			sharks[i].theta+=10.0;
		}
		else{
			sharks[i].theta=ttheta;
		}
		sharks[i].dtheta=sharks[i].theta-thetal;

		if(sharks[i].psi-tpsi>20.0){
			sharks[i].psi-=10.0;
			if (sharks[i].psi < -180.0) {
				sharks[i].psi += 360.0;
			}
		}
		else if(sharks[i].psi-tpsi<-20.0){
			sharks[i].psi+=10.0;
			if (sharks[i].psi > 180.0) {
				sharks[i].psi -= 360.0;
			}
		}
		else{
			sharks[i].psi=tpsi;
		}

		sharks[i].x += SHARKSPEED * sharks[i].v * cos(sharks[i].psi / RAD) * cos(sharks[i].theta / RAD);
		sharks[i].y += SHARKSPEED * sharks[i].v * sin(sharks[i].psi / RAD) * cos(sharks[i].theta / RAD);
		sharks[i].z += SHARKSPEED * sharks[i].v * sin(sharks[i].theta / RAD);
	}
	if(abs(sharks[i].x-sharks_org[i].x)<1000.0 && abs(sharks[i].y-sharks_org[i].y)<1000.0
	   && abs(sharks[i].z-sharks_org[i].z)<1000.0){
		shark_moving[i]=3;
	}
}

void shark_movingf(void){
	int i;
	for(i=0;i<NUM_SHARKS;i++){
		shark_moving[i]=1;
	}
}

void RotateShark(int i){
	sharks[i].psi=sharks_org[i].psi;
	shark_moving[i]=0;
}

