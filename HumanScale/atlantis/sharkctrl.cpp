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
		r=sqrt((TargetPos[0]-sharks[i].x)*(TargetPos[0]-sharks[i].x)+(TargetPos[1]-sharks[i].y)*(TargetPos[1]-sharks[i].y)
			+(TargetPos[2]-sharks[i].z)*(TargetPos[2]-sharks[i].z));
		if(r<rmin){
			rmin=r;
			lmin=i;
		}
	}
	return lmin;
}

void MoveShark(int i)
{
	float X,Y,Z,ttheta,tpsi,thetal;

	sharks[i].v=2.0;
	X=TargetPos[0]-sharks[i].x;
	Y=TargetPos[1]-sharks[i].y;
	Z=TargetPos[2]-sharks[i].z;
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
				//sharks[i].psi += 360.0;
			    sharks[i].psi = 0.0;
			}
	}
	else if(sharks[i].psi-tpsi<-20.0){
		sharks[i].psi+=10.0;
		if (sharks[i].psi > 180.0) {
			//sharks[i].psi -= 360.0;
			sharks[i].psi = 0.0;
		}
	}
	else{
		sharks[i].psi=tpsi;
	}

	sharks[i].x += SHARKSPEED * sharks[i].v * cos(sharks[i].psi / RAD) * cos(sharks[i].theta / RAD);
	sharks[i].y += SHARKSPEED * sharks[i].v * sin(sharks[i].psi / RAD) * cos(sharks[i].theta / RAD);
	sharks[i].z += SHARKSPEED * sharks[i].v * sin(sharks[i].theta / RAD);

   /* double len  = abs((TargetPos[0]-sharks[i].x)*(TargetPos[0]-sharks[i].x))+abs((TargetPos[1]-sharks[i].y)*(TargetPos[1]-sharks[i].y));
    len = len + abs((TargetPos[2]-sharks[i].z)*(TargetPos[2]-sharks[i].z));
	len =sqrt(len);

	if(len <= DIS_THRESHOLD){
		count[i]=0;
		sharks[i].status=2;
	}*/

	if(sharks[i].x < DIS_THRESHOLD){
		count[i]=0;
		sharks[i].status= SHARK_RETURN;
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
				li=sqrt((sharks[i].x-TargetPos[0])*(sharks[i].x-TargetPos[0])
						+(sharks[i].y-TargetPos[1])*(sharks[i].y-TargetPos[1])
						+(sharks[i].z-TargetPos[2])*(sharks[i].z-TargetPos[2]));
				lj=sqrt((sharks[j].x-TargetPos[0])*(sharks[j].x-TargetPos[0])
						+(sharks[j].y-TargetPos[1])*(sharks[j].y-TargetPos[1])
						+(sharks[j].z-TargetPos[2])*(sharks[j].z-TargetPos[2]));
				if(li>lj) sharks[i].v=0.0;
				else sharks[j].v=0.0;
			}
		}
	}
}

void ReturnShark(int i)
{

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
				//sharks[i].psi += 360.0;
				sharks[i].psi = 0.0;
			}
		}
		else if(sharks[i].psi-tpsi<-20.0){
			sharks[i].psi+=10.0;
			if (sharks[i].psi > 180.0) {
				//sharks[i].psi -= 360.0; 
				sharks[i].psi = 0.0; //jeong
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
		sharks[i].status= SHARK_ROTATE;
	}
}

void shark_movingf(void){
	int i;
	for(i=0;i<NUM_SHARKS;i++){
		sharks[i].status= SHARK_TARGET;
	}
}

void RotateShark(int i){
	sharks[i].psi=sharks_org[i].psi;
	sharks[i].status= SHARK_MOVE;
}

