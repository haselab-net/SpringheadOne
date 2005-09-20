#include "His/HISpidar4Quad.h"
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>
#include <Device/DRUsb20Simple.h>
#include <conio.h>
#include <iostream>

//追加分
#include <windows.h>
#include<mmsystem.h>
void CALLBACK MyTimeProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD);
void TimeFunc(void);
int flag = 0;
UINT FTimerId;
int check = 0;
int endcount = 0;
double tt = 0, ttt =0;
int i = 0;

LVRM::HISpidar4quad spidar;
LVRM::HISpidar4quad spidar2;

//追加分終了

const float K = 2000;
using namespace LVRM;

void main(){

//マルチメディアタイマー始動
	int count = 0;

	timeBeginPeriod(1);
	FTimerId = timeSetEvent(1, 1, MyTimeProc, (DWORD)&count, TIME_PERIODIC);

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

	spidar.Init(devMan, motorPos[0], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);
	spidar2.Init(devMan, motorPos[1], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);


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

		std::cout << "main" << std::endl;
		if (endcount < 10000){
			endcount = endcount + 1;
		}
		else if(endcount >= 10000){
			std::cout << "メインのループが回った回数 = " << endcount << std::endl;
			std::cout << "割り込んだ回数 = " << check << std::endl;
			exit(0);
		}
	}


end:;
}

void CALLBACK MyTimeProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD)
{
	TimeFunc();
	(*(int*)dwUser)++;
		return;
}

void TimeFunc(void)
{
	if (flag == 0){
		flag = 1;
		check = check + 1; 
		ttt = timeGetTime() - tt;
	
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

		//処理速度を(msec)で表示
		std::cout << "処理にかかった時間："<< ttt << "msec" << std::endl;
		tt = timeGetTime();
		flag = 0;
	}else if(flag == 1){
		std::cout << "割り込まれた" << std::endl;
		}
} 

