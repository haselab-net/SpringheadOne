#include "HIS/HISpidar4Quad.h"
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>
#include <Device/DRUsb20Simple.h>
#include <conio.h>
#include <iostream>

const float K = 2000;
using namespace LVRM;

HISpidar4quad spidar;
HISpidar4quad spidar2;

void main(){

	DVDeviceManager devMan;
	devMan.RPool().Register(new DRUsb20Simple(0));
	/*
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	*/
	devMan.Init();

	std::cout << devMan;

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
//	#define PX	0.265f	//	x方向の辺の長さ/2
//	#define PY	0.265f	//	y方向の辺の長さ/2
//	#define PZ	0.265f	//	z方向の辺の長さ/2

	//	VM室Big-SPIDAR用　モータの取り付け座標（ターンテーブルの中心から床上110cmが原点）	
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.75f, 2.00f, 1.12f),	Vec3f( 1.65f, 2.17f,-1.34f), Vec3f(-1.65f,-0.9f,-1.44f), Vec3f( 1.65f,-0.99f, 0.50f)},
		{Vec3f( 1.58f, 2.00f, 1.12f),	Vec3f(-1.65f, 2.18f,-1.34f), Vec3f( 1.65f,-0.98f,-1.44f), Vec3f(-1.65f,-0.98f, 0.50f)}
	};

	spidar.Init(devMan, motorPos[0], 0.2f/*0.3776f*/, 2.924062107079e-5f, 1.0f, 20.0f);
	spidar2.Init(devMan, motorPos[1], 0.2f/*0.3776f*/, 2.924062107079e-5f, 1.0f, 20.0f);


	while(1){
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				goto end;
				break;
			case  0x36:
				spidar.Calib();
				spidar2.Calib();
				break;
			}
		}


/*		float rad = 0.25;
		Vec3f colde, colde2;
		Vec3f force, force2;

		Affinef graphic = Affinef(0, 0, -0.5);

		colde = graphic.Pos() - spidar.GetPos(); 
		colde2 = graphic.Pos() - spidar2.GetPos(); 
				
		spidar.Update();
		if (rad - colde.norm() > 0){
			force = -K * (rad - colde.norm()) * (graphic.Pos() - spidar.GetPos()).unit();
			}
			spidar.SetForce(force, 0.1f);

		spidar2.Update(); 
		if (rad - colde2.norm() > 0){
			force2 = -K * (rad - colde2.norm()) * (graphic.Pos() - spidar2.GetPos()).unit();
			}
			spidar2.SetForce(force2, 0.1f);
*/

		Vec3f force;
		spidar.Update();
		if (spidar.GetPos().X() < -0.5){
			force.X() = - K * (spidar.GetPos().X() + 0.5f);
		}
		if (spidar.GetPos().X() > 0.5){
			force.X() = - K * (spidar.GetPos().X() - 0.5f);
		}
		spidar.SetForce(force, 0.5f);

		
		Vec3f force2;
		spidar2.Update();
		if (spidar2.GetPos().X() < -0.5){
			force2.X() = - K * (spidar2.GetPos().X() + 0.5f);
		}
		if (spidar2.GetPos().X() > 0.5){
			force2.X() = - K * (spidar2.GetPos().X() - 0.5f);
		}
		spidar2.SetForce(force2, 0.5f);

//		std::cout << spidar.GetPos().Y() << std::endl;
//		std::cout << spidar2.GetPos().Y() << std::endl;
		
//		for(int i=0;i<4;i++){
//			std::cout << spidar.Motor()[i].Length() << "  ";
//		}
//		printf("\n");
		

	}


end:;
}

	
		//仮に球の位置をguraphic.Pos(),半径をradとする
		/*
		float rad = 0.25;
		Vec3f colde, colde2;
		Vec3f force;

		Affinef graphic = Affinef(0, 0, 0.5);

		colde = graphic.Pos() - spidar.GetPos(); 
		colde2 = graphic.Pos() - spidar2.GetPos(); 
		
		if (rad - colde.norm() > 0){
			force() = -K * (rad - cold.norm()) * (graphic.Pos() - spidar.GetPos()).unit();
			}
			spidar.SetForce(force, 0.1f);

		colde2 = graphic.Pos() - spidar2.GetPos(); 
		if (rad - colde.norm() > 0){
			force2() = -K * (rad - cold2.norm()) * (graphic.Pos() - spidar2.GetPos()).unit();
			}
			spidar2.SetForce(force2, 0.1f);

		*/
