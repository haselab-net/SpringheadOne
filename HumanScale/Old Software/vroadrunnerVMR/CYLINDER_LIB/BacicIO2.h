#ifndef _BACICIO2_H_
#define _BACICIO2_H_

#include <windows.h>
//#include <windowsx.h>
//#include <stdio.h>
#include <time.h>
#include <math.h>
#include <iostream>
//#include <string>
#include "usbdio32.h"
//#include "resource.h"

#define		WaitTime	1000	//停止時間(1秒)
#define		AllUp		0x00	//アクチュエータ全て上げる
#define		AllDown		0xff	//アクチュエータ全て下げる
#define		Cylinder1	0x01	//1
#define		Cylinder2	0x04	//4
#define		Cylinder3	0x10	//16
#define		Cylinder4	0x40	//64
#define		Cylinder5	0x05	//5
#define		Cylinder6	0x11	//17
#define		Cylinder7	0x14	//20
#define		PORT_Input		0x00
#define		PORT_Output		0xff
#define		ON			true
#define		OFF			false

const double PI = 3.14159265358979323846;

using namespace std;

class PIO
{
	
public:
	int CylinderNum;
	USHORT UnitId,PortNum,Dir;
	bool cylinder_Status[3];  // 各シリンダの状態を保持（ON : true, OFF : false)
	PIO(){CylinderNum=0xff,UnitId=0x00;PortNum=PORT0;Dir=PORT_Output;}
	~PIO();

	//int getCylinderNum() const {return CylinderNum;};

	//PIOのための必要初期設定(接続機器が１つで、出力のみ利用なら変更しなくてよい)
	void Init(USHORT UnitId,USHORT PortNum,USHORT Dir);
	//指定シリンダをON/OFF
	void Output(int CylinderNum2, bool OnOff);
	//指定時間待つ
	int Wait(int T);
	//Unitの状態確認用
	void report(){
		cout<<"UnitIdは"<<UnitId<<" Port番号は"<<log(PortNum)/log(2)<<" Dirは"<<Dir<<endl;
		cout<<endl;
	}
	//シリンダの動作制御
	void Control3(float angle, bool active, bool isJump);
};
#endif // _BACICIO2_H_
