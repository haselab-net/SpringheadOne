#include "HIS/HISpidar4Quad.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <Device/DRUsbH8Simple.h>
#include <conio.h>
#include <iostream>
#include <fstream>

const float K = 2000;
using namespace LVRM;

class HISpidarDump:public HISpidar4quad{
public:
	void SetPos(Vec3f p){ pos = p;}
	float* RealTens(){ return realtens; }
};

void main(){
	DVDeviceManager devMan;
	HISpidarDump spidar;

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	spidar.Init(devMan, motorPos[0], 0.5776f, 2.924062107079e-5f, 1.0f, 20.0f);
	Vec3f force(5,5,5);
	std::ofstream file[10];
	float eff[] = {0.0f,	0.0f,	1.0E-6f,0.0f, 0.01f, 0.01f , 0.01f };
	float cont[] = {0.0f,	1.0E-6f,0.0f,	0.1f, 0.1f,  0.2f, 1.0f};
	for(int i=0; i<sizeof(eff)/sizeof(eff[0]); i++){
		char name[1024];
		strcpy(name, "F0.txt");
		name[1] = i+'0';
		file[i].open(name);
		for (float x=-PX; x<=PX; x+=0.001f){
			spidar.SetPos(Vec3f(x,x,x));
			spidar.SetForce(force, eff[i], cont[i]);
			file[i] << x;
			for(int j=0; j<4; j++){
				file[i] << ", " << spidar.RealTens()[j];
			}
			file[i] << ", " << spidar.GetForce().X();
			file[i] << std::endl;
		}
	}
}
