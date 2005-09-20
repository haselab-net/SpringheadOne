#include "HIS/HISpidar4Quad.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <Device/DRUsbH8Simple.h>
#include <WinBasis/WBMMTimer.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace LVRM;
const float K = 2000;
Vec3f force;
volatile float p,f;
float rambda = 1.0f;

void step(void* arg){
	HISpidar4quad& spidar = *(HISpidar4quad*)arg;
	spidar.Update();
	p = spidar.GetPos().X();
	spidar.SetForce(force, rambda);
	f = spidar.GetForce().X();
}
void main(){
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	devMan.Init();
	HISpidar4quad spidar;
	std::cout << devMan;

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.31f	//	x方向の辺の長さ/2
	#define PY	0.24f	//	y方向の辺の長さ/2
	#define PZ	0.24f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
	spidar.Init(devMan, motorPos[0], 0.3653f, 2.924062107079e-5f, 1.0f, 20.0f);

	WBMMTimer timer;
	timer.Thread();
	timer.Set(step, &spidar);
	bool bWrite = false;
	std::ofstream file;
	while(1){
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				return;
				break;
			case 't':
				timer.Create();
				break;
			case 'T':
				timer.Thread();
				break;
			case 'f':
				force = Vec3f(5,5,5);
				break;
			case 'F':
				force = Vec3f(0,0,0);
				break;
			case 'w':
				if (!bWrite){
					static int count;
					char fn[1024];
					sprintf(fn, "data%d.txt", count++);
					file.close();
					file.clear();
					file.open(fn);
					bWrite = true;
					std::cout << fn << "...";
					file << "rambda = " << rambda << std::endl;
				}else{
					std::cout << "stop to write." << std::endl;
					bWrite = false;
				}
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				rambda = (ch-'0') / 10.0f;
				break;
			case '0':
				rambda = 1E-6f;
				break;
			case '-':
				rambda = 1.0f;
				break;
			case '^':
				rambda = 2.0f;
				break;
			}
		}
		if (bWrite){
			file << GetTickCount()  << ", "
				<< p << ", " << f << std::endl;
			Sleep(1);
		}
	}
} 
