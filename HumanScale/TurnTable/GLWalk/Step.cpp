// Step.cpp: Step �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "OpenGL.h"

#include "Step.h"
#include "Tuadapio.h"
#include <math.h>
#include <iostream.h>
#include <mmsystem.h>

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Step::Step()
{
	d_grav = walkvel = 0;
	step = 0;
	for(int i = 0 ; i < 6 ; i++ )
	{
	grav_x[i] = 0;
	grav_y[i] = 0;
	}
	dwStepTime = timeGetTime();
}

Step::~Step()
{

}

void Step::WalkInStep()
{

	short dat[3];//�f�[�^�z��
	int ch;//�`�����l��
	float right,left,front;

	for(ch = 0 ; ch < 3 ; ch++ )
	{
	Adapio_Adc_SingleSample(10,ch,&dat[ch]);
	}
	right = (float)(dat[0] - 104) / 230;//�����Ԃ�[�����i�E���̃Z���T�j
	left = (float)(dat[1] - 150) / 250;	//����i�����̃Z���T�j
	front = (float)(dat[2] - 60) / 200;	//����i�O�̃Z���T�j

	grav_x[5] = 0.866 * (right - left);	// (��3 / 2) = 0.86602540378443864676372317075294���
	grav_y[5] = front - 0.5 * (right + left);
//	5��O�ɃT���v�����O�����l�Ƃ̔�r�����āA�d�S�̕ω��̑傫����d_grav�Ƃ���
	d_grav = sqrt((grav_x[5] - grav_x[0]) * (grav_x[5] - grav_x[0]) + (grav_y[5] - grav_y[0]) * (grav_y[5] - grav_y[0]));

	dwInterval = timeGetTime() - dwStepTime;
	if (d_grav > 0.1f && dwInterval >= 300)
	{
		step++;
//		���� = (0.2 * pitch + 0.5)	���_�̃f�[�^���Q�ƁBpitch = �P�b������̕���
		pitch = (float) 1000 / dwInterval ;	
		walkvel = (float) (0.2 * pitch + 0.5) * pitch;
		dwStepTime = dwStepTime + dwInterval;
	}else if(dwInterval * pitch > 1000)	//�P�����A��������
		walkvel = 0;					//���s���x���O�ɖ߂�

	for(int i = 0 ; i < 5 ; i++ )
	{
	grav_x[i] = grav_x[i + 1];
	grav_y[i] = grav_y[i + 1];
	}

//	cout <<"d_grav = " << d_grav << endl;
	cout <<"step = " << step << "walk velocity = " << walkvel << endl;


}
