
/************************************/
/*     補間法（３次スプライン関数） */
/************************************/
#include <stdio.h>
#include "Spline.h"
/********************/
/*     クラスの定義 */
/********************/

/******************************/
/*     コンストラクタ         */
/*          n1 : 区間の数     */
/*          x1, y1 : 点の座標 */
/******************************/
Spline::Spline()
{
}

/********************/
/*     デストラクタ */
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
/*     ３次スプライン関数による補間   */
/*          n : 区間の数              */
/*          x, y : 点の座標           */
/*          x1 : 補間値を求める値     */
/*          h, b, d, g, u, r : 作業域 */
/*          return : 補間値           */
/**************************************/

/**********************************/
/*     補間値の計算               */
/*          x1 : 補間値を求める値 */
/*          return : 補間値       */
/**********************************/
double Spline::value(double x1)
{
	int i = -1, i1;
	double y1, xx;
				// 区間の決定
	for (i1 = 1; i1 < n && i < 0; i1++) {
		if (x1 < x[i1])
			i = i1 - 1;
	}
	if (i < 0)
		i = n - 1;
				// 計算
	xx = x1 - x[i];
	y1 = y[i] + xx * (q[i] + xx * (r[i]  + s[i] * xx));

	return y1;
}


void Spline::run(int n, double *x, double *y)
{
	double sp;//, x1, y1;
	//int n;
					// 設定
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
					// 実行と出力

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
