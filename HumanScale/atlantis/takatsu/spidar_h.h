#include <Device/DRUsb20Simple.h>
#include <HIS/HISpidar4Quad.h>
#include <winBasis/WBMMTimer.h>
#include <conio.h>
#include <iostream>

using namespace LVRM;

bool bMinForce;
Vec3f H_force[2];		//	�񎦂����
const float K = 10.0f;	
WBMMTimer timer;
#define MAXON	0.3776f
#define LPP		2.924062107079e-5f
#define SpidarUpdateRate	1
//------------------------------------------------------------------------
//			static float pos_x_buf1=0,pos_x_buf2=0,pos_x_buf3=0;
//------------------------------------------------------------------------
//spidar.Update(0.001f);				//	�A���v�Ƃ̒ʐM(�͂�񎦁E�ʒu���擾)
//1khz 0.001f

	DVDeviceManager devMan;		//	�f�o�C�X�h���C�o�����Ă������ꕨ
   
	HISpidar4quad h_spidar[2];

	Vec3f grp_pos[2];

	void spidarInit();
	void spidarCheck();

