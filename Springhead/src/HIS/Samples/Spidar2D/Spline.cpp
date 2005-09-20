
/************************************/
/*     ��Ԗ@�i�R���X�v���C���֐��j */
/************************************/
#include <stdio.h>
#include "Spline.h"
/********************/
/*     �N���X�̒�` */
/********************/

/******************************/
/*     �R���X�g���N�^         */
/*          n1 : ��Ԃ̐�     */
/*          x1, y1 : �_�̍��W */
/******************************/
Spline::Spline()
{
}

/********************/
/*     �f�X�g���N�^ */
/********************/
Spline::~Spline()
{
	delete [] h;
	delete [] b;
	delete [] d;
	delete [] g;
	delete [] u;
	delete [] q;
	delete [] s;
	delete [] r;
	delete [] x;
	delete [] y;
}

/**************************************/
/*     �R���X�v���C���֐��ɂ����   */
/*          n : ��Ԃ̐�              */
/*          x, y : �_�̍��W           */
/*          x1 : ��Ԓl�����߂�l     */
/*          h, b, d, g, u, r : ��ƈ� */
/*          return : ��Ԓl           */
/**************************************/

/**********************************/
/*     ��Ԓl�̌v�Z               */
/*          x1 : ��Ԓl�����߂�l */
/*          return : ��Ԓl       */
/**********************************/
double Spline::value(double x1)
{
	int i = -1, i1;
	double y1, xx;
				// ��Ԃ̌���
	for (i1 = 1; i1 < n && i < 0; i1++) {
		if (x1 < x[i1])
			i = i1 - 1;
	}
	if (i < 0)
		i = n - 1;
				// �v�Z
	xx = x1 - x[i];
	y1 = y[i] + xx * (q[i] + xx * (r[i]  + s[i] * xx));

	return y1;
}


void Spline::run(int n, double *x, double *y)
{
	double sp;//, x1, y1;
	//int n;
					// �ݒ�
	//n = 5;
	x = new double [n+1];
	y = new double [n+1];

/*
	x[0] = 0.0;
	x[1] = 4.0;
	x[2] = 2.0;
	x[3] = 3.0;
	x[4] = 4.0;
	x[5] = 5.0;
	y[0] = 0.0;
	y[1] = 1.0;
	y[2] = 4.0;
	y[3] = 9.0;
	y[4] = 16.0;
	y[5] = 25.0;
*/
		
//	Spline spline(n, x, y);
					// ���s�Əo��

	//while(repetition_n<5)
	//{
	//}


	double x1 = 0.0;
	sp = 0.1;
	while (x1 <= 15.01) {
		double y1 = value(x1);
		printf("%f %f\n", x1, y1);
		x1 += sp;
	}
	delete [] x;
	delete [] y;

}

void Spline::initialization()
{

}
