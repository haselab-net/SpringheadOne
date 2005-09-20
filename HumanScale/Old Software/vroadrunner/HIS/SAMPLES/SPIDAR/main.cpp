#include "His/HISpidar4Quad.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <Device/DRUsbH8Simple.h>
#include <conio.h>
#include <iostream>

const float K = 2000;
using namespace LVRM;

void main(){
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	devMan.Init();
	HISpidar4quad spidar;
	HISpidar4quad spidar2;
	std::cout << devMan;

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	spidar.Init(devMan, motorPos[0], 0.5776f, 2.924062107079e-5f, 1.0f, 20.0f);
	spidar2.Init(devMan, motorPos[1], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);

	while(1){
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				goto end;
				break;
			}
		}
		Vec3f force;
		spidar.Update();
		if (spidar.GetPos().Y() < 0){
			force.Y() = - K * spidar.GetPos().Y();
		}
		spidar.SetForce(force);

		Vec3f force2;
		spidar2.Update();
		if (spidar2.GetPos().Y() < 0){
			force2.Y() = - K * spidar2.GetPos().Y();
		}
		spidar2.SetForce(force2);

/*
		for(int i=0; i<4; i++){
			std::cout << spidar.Motor()[i].Length() << " ";
		}
		for(int i=0; i<4; i++){
			std::cout << spidar2.Motor()[i].Length() << " ";
		}
		std::cout << std::endl;
*/
} 
end:;
}
