#include <HIS/HISpidarG6X.h>
#include <HIS/HISpidarG6X3.h>
#ifdef __sh__		//	SH4版(コントローラのファーム内で動作する場合)
 #include <Device/DRIhcSh4.h>
 #include <cyg/kernel/kapi.h>
#endif
#ifdef _WIN32		//	Win32版(普通はこっち)
 #include <Device/DRUsb20Simple.h>
 #include <Device/DRUsb20Sh4.h>
 #include <conio.h>
#endif
#include <iostream>
#include <stdio.h>

#ifdef __sh__
const float K = 20000;
const float B = 20;
const float dt = 0.0001f;
#else
const float K = 2000;
const float B = 0;
const float dt = 0.001f;
#endif
using namespace Spr;

int main(){
	DVDeviceManager devMan;						//	D/Aやカウンタなどのデバイスの管理
#ifdef __sh__
	devMan.RPool().Register(new DRIHCSh4);		//	SH4からアクセスするコントローラのI/O
#else
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(1));		//	Sh4版コントローラ 8モータ
	devMan.RPool().Register(new DRUsb20Sh4(2));		//	Sh4版コントローラ 8モータ
//	devMan.RPool().Register(new DRUsbH8Simple(0));	//	H8版コントローラ 8モータ
/*	devMan.RPool().Register(new DRContecIsaDa(0x300));	//	ISAボード版(3つで8モータ用)
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));	*/
#endif
	devMan.Init();								//	デバイスの初期化
	std::cout << devMan;						//	初期化の結果を表示

	HISpidarG6X3 spidarG6;						//	SPIDARに対応するクラス
/*
	std::cout << "hoge" << std::endl;
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	(0.235f/2)		//	x方向の辺の長さ/2
	#define PY	(0.225f/2)		//	y方向の辺の長さ/2
	#define PZ	(0.235f/2)		//	z方向の辺の長さ/2
	//	糸のグリップへの取り付け位置
	#define GX	(0.0555f/2)		//	x方向の辺の長さ/2
	#define GY	(0.0555f/2)		//	y方向の辺の長さ/2
	Vec3f motorPos[8][2] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ), Vec3f(-GX,  0, 0)},
		{Vec3f( PX, PY,-PZ), Vec3f(  0, GY, 0)},
		{Vec3f(-PX,-PY,-PZ), Vec3f(  0,-GY, 0)},
		{Vec3f( PX,-PY, PZ), Vec3f( GX,  0, 0)},
		{Vec3f( PX, PY, PZ), Vec3f( GX,  0, 0)},
		{Vec3f(-PX, PY,-PZ), Vec3f(  0, GY, 0)},
		{Vec3f( PX,-PY,-PZ), Vec3f(  0,-GY, 0)},
		{Vec3f(-PX,-PY, PZ), Vec3f(-GX,  0, 0)}
	};
	spidarG6.Init(devMan, 8, motorPos, 0.3f, 2.924062107079e-5f, 1.0f, 20.0f);
	for(int i=4; i<8; ++i){
		spidarG6.motor[i].lengthPerPulse *= -1;
	}
	spidarG6.Calib();
*/
	spidarG6.Init(devMan, true);			//	SPIDARの初期化，キャリブレーションもする．
#ifdef __sh__
	cyg_thread_delay(10000);
#else
	Sleep(1000);					
#endif
	spidarG6.Calib();				//	キャリブレーション

#define NVTX	4					//	衝突判定する点
	Vec3f grip[NVTX]={
		Vec3f( 0.02f, 0.0f, 0.02f),
		Vec3f(-0.02f, 0.0f, 0.02f),
		Vec3f( 0.02f, 0.0f,-0.02f),
		Vec3f(-0.02f, 0.0f,-0.02f)
//		Vec3f()
	};
	float lastPos=0;
	float vel=0;
	while(1){
#ifdef _WIN32
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				return 0;
			}
		}
#endif
		spidarG6.Update(dt);						//	USBの通信を行う．
#if 0
		Vec3f pos = spidarG6.GetPos();				//	位置の読み出し Vec3fについては，自動生成マニュアルを参照
		float delta = (pos.Y() - lastPos) / dt;
		float alpha = 0.1f;
		vel = (1-alpha)*vel + alpha*delta;
		lastPos = pos.Y();
		Quaternionf qt = spidarG6.GetOri();			//	角度の読み出し 4元数 Quaternionf については，自動生成マニュアルを参照
		float force=0;
		Vec3f torque;
		for(int i=0; i<NVTX; ++i){
			Vec3f lp = qt*grip[i];
			Vec3f p = lp + pos;
			const float WALL = -0.001f;
			if (p.Y() < WALL){
				force += (WALL-p.Y())*K + B*vel;
				torque += lp ^ Vec3f(0, force, 0);
			}
		}
		spidarG6.SetForce(Vec3f(0, force, 0), torque);	//	力の取得
#endif
		
#if 0
	std::cout << spidarG6.GetPos() << std::endl;
#endif
#if 1
		for( int i=0; i<8; i++ ) std::cout << spidarG6.motor[i].GetLength() << " ";
		std::cout << std::endl;
#endif
	}
	return 0;
}
