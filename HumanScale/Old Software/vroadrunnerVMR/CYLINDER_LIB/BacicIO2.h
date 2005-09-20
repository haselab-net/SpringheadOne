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

#define		WaitTime	1000	//��~����(1�b)
#define		AllUp		0x00	//�A�N�`���G�[�^�S�ďグ��
#define		AllDown		0xff	//�A�N�`���G�[�^�S�ĉ�����
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
	bool cylinder_Status[3];  // �e�V�����_�̏�Ԃ�ێ��iON : true, OFF : false)
	PIO(){CylinderNum=0xff,UnitId=0x00;PortNum=PORT0;Dir=PORT_Output;}
	~PIO();

	//int getCylinderNum() const {return CylinderNum;};

	//PIO�̂��߂̕K�v�����ݒ�(�ڑ��@�킪�P�ŁA�o�͂̂ݗ��p�Ȃ�ύX���Ȃ��Ă悢)
	void Init(USHORT UnitId,USHORT PortNum,USHORT Dir);
	//�w��V�����_��ON/OFF
	void Output(int CylinderNum2, bool OnOff);
	//�w�莞�ԑ҂�
	int Wait(int T);
	//Unit�̏�Ԋm�F�p
	void report(){
		cout<<"UnitId��"<<UnitId<<" Port�ԍ���"<<log(PortNum)/log(2)<<" Dir��"<<Dir<<endl;
		cout<<endl;
	}
	//�V�����_�̓��쐧��
	void Control3(float angle, bool active, bool isJump);
};
#endif // _BACICIO2_H_
