#include "HIS/HISpidar4Quad.h"
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>
#include <Device/DRUsb20Simple.h>
#include <conio.h>
#include <iostream>

const float K = 1000;
//const float K = 300;	// D-Vision BigSpidar ではこんなもん
using namespace LVRM;

HISpidar4quad spidar;
HISpidar4quad spidar2;

////////// ちょい実験 //////////
Vec3f CalcAcceleration(Vec3f* posLog, Vec3f pos){
	Vec3f acce = Vec3f(0.0, 0.0, 0.0);
	for(int i = 0; i < 2; i++) posLog[i+1] = posLog[i];
	posLog[0] = pos;
	acce = (posLog[0] - 2 * posLog[1] + posLog[2]);
	return acce;
}
////////// ここまで //////////

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
	//#define PX	0.265f	//	x方向の辺の長さ/2
	//#define PY	0.265f	//	y方向の辺の長さ/2
	//#define PZ	0.265f	//	z方向の辺の長さ/2

	// 動作確認SPIDAR用 モータの取り付け座標
	#define PX	0.3125f			//	x方向の辺の長さ/2 [m]
	#define PY	0.23f			//	y方向の辺の長さ/2 [m]
	#define PZ	0.265f			//	z方向の辺の長さ/2 [m]
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}		
	};
	//	VM室Big-SPIDAR用　モータの取り付け座標（ターンテーブルの中心から床上110cmが原点）	
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)	元データ
		{Vec3f(-1.75f, 2.00f, 1.12f),	Vec3f( 1.65f, 2.17f,-1.34f), Vec3f(-1.65f,-0.9f,-1.44f), Vec3f( 1.65f,-0.99f, 0.50f)},
		{Vec3f( 1.58f, 2.00f, 1.12f),	Vec3f(-1.65f, 2.18f,-1.34f), Vec3f( 1.65f,-0.98f,-1.44f), Vec3f(-1.65f,-0.98f, 0.50f)}
	};*/
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};*/

	spidar.Init (devMan, motorPos[0], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);
	spidar2.Init(devMan, motorPos[1], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);

	bool bMinForce = true;
	int step = 0;
	Vec3f posLog[3];
	for(int i = 0; i < 3; i++) posLog[i] = Vec3f(0.0, 0.0, 0.0);
	while(1){
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:	// Esc でMinForce にして終了
				printf("SetMinForce\n");
				spidar.SetMinForce();
				spidar2.SetMinForce();
				goto end;
				break;
			case  'c':	// 'c' でキャリブレーション
				spidar.Calib();
				spidar2.Calib();
				break;
			case  ' ':	// ' ' で Force の有無切り替え
				bMinForce = !bMinForce;
				break;
			}
		}

		Vec3f force;
		spidar.Update();
		
		//	取り合えず壁
		if (spidar.GetPos().X() < -0.1){
			force.X() = - K * (spidar.GetPos().X() + 0.1f);
		}
		if (spidar.GetPos().X() > 0.1){
			force.X() = - K * (spidar.GetPos().X() - 0.1f);
		}
		Vec3f acce, forceM;
		if((acce = CalcAcceleration(posLog, spidar.GetPos())).norm() > 0.0001){
			forceM = (-1) * acce.unit() * force.norm() * 0.5f;
			force += forceM;
		}
		printf("(%f, %f, %f)\n", forceM.X(), forceM.Y(), forceM.Z());

		if(bMinForce) spidar.SetMinForce();
		else          spidar.SetForce(force, 0.5f);

		Vec3f force2;
		spidar2.Update();

		//	取り合えず壁
		if (spidar2.GetPos().X() < -0.1){
			force2.X() = - K * (spidar2.GetPos().X() + 0.1f);
		}
		if (spidar2.GetPos().X() > 0.1){
			force2.X() = - K * (spidar2.GetPos().X() - 0.1f);
		}

		if(bMinForce) spidar2.SetMinForce();
		else          spidar2.SetForce(force2, 0.5f);
		
		if(step == 100){
			step = 0;
			//	SPIDAR の座標表示
			//printf("1: (x,y,z) = (%f, %f, %f)\n",  spidar.GetPos().X(),  spidar.GetPos().Y(),  spidar.GetPos().Z());
			//printf("2: (x,y,z) = (%f, %f, %f)\n\n",spidar2.GetPos().X(), spidar2.GetPos().Y(), spidar2.GetPos().Z());
			//printf("\n");

			//	モーターの糸の長さ表示
			/*for(int i=0;i<4;i++){
				std::cout << i   << ": " << spidar.Motor()[i].Length()  << std::endl;
			}
			for(int i=0;i<4;i++){
				std::cout << i+4 << ": " << spidar2.Motor()[i].Length() << std::endl;
			}
			printf("\n");*/
		}
		step++;
	}
	end:;
}
