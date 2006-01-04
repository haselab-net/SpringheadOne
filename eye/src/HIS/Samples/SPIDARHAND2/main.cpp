#include <His/HISpidar4.h>
#include <Device/DRUsb20Simple.h>
#include <conio.h>
#include <iostream>
#include "HISpidarHand.h"
#include "HISpidarFinger.h"

const float K = 2000;
using namespace Spr;
using namespace std;

void main(){
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();
	HISpidar4 spidar;
	HISpidarHand aHand;

	std::cout << devMan;

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
	Vec3f fingerPos[][4] = {	//	指の曲げ始め位置
		{Vec3f(-0.04f, 0, -0.07f),Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};


	spidar.Init(devMan, motorPos[0], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);
	aHand.InitInstance(devMan);
	//最小張力の発生と指の初期位置の追加
	aHand.Init(fingerPos[0],0.4237288f, 0.9203865e-5f, 0.1f, 1.0f);
	cout << aHand.m_pFinger[1]->InitFingerPos1.X()<<endl;

	for(int i=0;i<3;i++){
		aHand.m_pFinger[i]->FingerCalib();
	}

	for(int j=0;j<3;j++){
		aHand.m_pFinger[j]->FingerCalib1();
	}

	while(1){

		/*spidar.Update();
		cout << spidar.Motor()[0].counter->Count() << endl;
		cout << aHand.motor[0].counter->Count() << endl;
		//手の位置座標を表示
		cout << aHand.GetPos().X() <<" "<<aHand.GetPos().Y() << " "<<aHand.GetPos().Z()<<endl;
		*/
		//cout <<"手の位置座標"<< spidar.GetPos().X()<<" "<<spidar.GetPos().Y()<<" "<<spidar.GetPos().Z()<<endl;
		//cout <<"モータのエンコーダ値"<< aHand.motor[0].counter->Count()<<endl;
		//cout <<"指のY座標"<< aHand.m_pFinger[0]->FingerPos1.Y()<<endl;
		static int count = 0;
		count ++;
		if (count > 100){
			if (kbhit()){
				int ch = getch();
				switch(ch){
				case 0x1b://escキー
					goto end;
					break;
				case 0x30://"1"キー（指を90°曲げて）
					for(int i=0;i<3;i++){
						aHand.m_pFinger[i]->FingerCalib();
					}
					break;
				case 0x31://"2"キー（指をまっすぐ伸ばして）
					for(int j=0;j<3;j++){
						aHand.m_pFinger[j]->FingerCalib1();
					}
					break;
				}
			}
			cout <<"糸の長さ"<<aHand.m_pFinger[0]->FingerLength1<<endl;
			count = 0;
		}

		//float force = 10 * (aHand.m_pFinger[0]->FingerPos1.Y()-0.03);
		float diff = aHand.m_pFinger[0]->FingerLength1 - 0.01;
		float force = 0.1f;
		if (diff > 0) {
			force += diff * 1.0f;
		}
		aHand.m_pFinger[0]->m_pMotor1[0].SetForce(force);

		//各値を更新
		aHand.Update();	
		for(int i=0;i<3;i++){
			aHand.m_pFinger[i]->Update();
		}
		
	}//end while
	end:;
}
