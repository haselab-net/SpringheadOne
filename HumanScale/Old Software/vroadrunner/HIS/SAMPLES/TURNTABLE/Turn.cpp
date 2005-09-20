// Turn.cpp: Turn クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "Turn.h"
#include "Tuadapio.h"
#include "Tokin.h"
#include <mmsystem.h>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Turn::Turn(){
	Init();
}

Turn::~Turn(){
	Uninit();
}

void Turn::Init()
{
	tokin = new Tokin;
	tokin->Init_tokin();
	tokin->SetStdData();
	tokin->Poll();
	tokin->GetDeviceData();
	start_angle = tokin->angle;
	Adapio_Device_Open(10);
	cout << "Adapio device (10) open" << endl;
	v_turn = 0;
	dangle = 0;
	Adapio_Dac_Out(10,0,V_CENTER);
	dwTime = timeGetTime();
}

void Turn::Uninit()
{
	Adapio_Device_Close(10);
	cout << "Adapio device (10) close" << endl;
	tokin->Uninit_tokin();
	delete tokin;
}

void Turn::GetAngle()
{
	tokin->GetDeviceData();
	if(tokin->retinfo == 0x00 )
	{
		now_angle = -(tokin->angle - start_angle);
		cout <<"Angle = " << now_angle << endl;
//		v_turn = - (char)(50 * now_angle)で回転させる。50は仮の値
		const float acc = 2.0;
		const float dec = 4.0;
		if(now_angle > 0.1)
		{
			if(v_turn > - (70 * now_angle))
				v_turn -= acc;
			if(v_turn < - (70 * now_angle))
				v_turn += acc;
		}
		else if(now_angle < -0.1)
		{
			if(v_turn < - (70 * now_angle))
				v_turn += acc;
			if(v_turn > - (70 * now_angle))
				v_turn -= acc;
		}
		else if(v_turn < 0)
			v_turn += dec;
		else if(v_turn > 0)
			v_turn -= dec;

		if (v_turn>127) v_turn = 127;
		if (v_turn<-128) v_turn = -128;
		Adapio_Dac_Out(10,0,V_CENTER + (int)v_turn);

//		v_turn = 0xff = 255 = 128(中心) + 127 のとき、ターンテーブルは時計回りに19.2[rpm]
//		つまり、v_turn=1につき  19.2 * 2π[rad] / 60[sec] / 127 = 0.015831648
//		v_turn = 0x00 = 0 = 128(中心) - 128 のとき、ターンテーブルは反時計回りに19.3[rpm]
//		つまり、v_turn=1につき  19.3 * 2π[rad] / 60[sec] / 128 = 0.015789776
//		よって、v_turn=1につき、0.0158[rad/sec]の速度で回転するとして計算する。
	
		DWORD dt;
		dt = timeGetTime() - dwTime;
		dwTime = timeGetTime();		
//		dangle = - 0.0158 * v_turn * dt / 1000;	
		dangle = - 0.0158 * v_turn * 0.02;	
	}		
}



void Turn::ResetAngle()
{
	tokin->GetDeviceData();
	start_angle = tokin->angle;
}
