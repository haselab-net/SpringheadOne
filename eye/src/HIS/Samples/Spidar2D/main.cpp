//#include "His/HISpidar4.h"
#include "HISpidar2.h"

//	USB2.0アンプのドライバ
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
#include <WinBasis/WBMMTimer.h>
//	ContectのISAカードのドライバ．古いもの(1997-)．
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>

#include <conio.h>
#include <iostream>

using namespace Spr;	//	SprはSpringheadライブラリの名前空間

#include <windows.h>
#include "Graph.h"

// グローバル変数 ----------------------------
//extern int x_target_point;
//extern int y_target_point;
// (main用)マウスで指定した位置
double X_M;
double Y_M;
// -------------------------------------------

bool bMinForce = true;	//	いきなり力を出すと危険なので，スペースを押すまでは最小張力を出力
void TimerFunc(void*);

//----------------------------------------------------------------------------------		
/*	4本糸のSPIDAR.
		糸の長さから座標計算を行ったり、出力する力覚から各モータの
		出力を計算したりする。出力の計算に2次計画法を用いているため，
		HIS/Fortran の libF77.lib libI77.lib QuadPR.lib が必要．
	@verbatim
	SPIDARの定数の設定例
	Init(dev, motPos, 0.5776, 2.924062107079e-5f, 0.5f, 20.0f);

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] =	//	モータの取り付け位置(中心を原点とする){
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	//	１ニュートン出すためのDAの出力電圧[V/N]
	//	maxon:		0.008[m] / 0.0438[Nm/A] / 0.5[A/V] =  0.365296803653
	//	千葉精機:	0.008[m] / 0.0277[Nm/A] / 0.5[A/V] =  0.5776
	//	maxonφ35:	0.008[m] / 0.0389[Nm/A] / 0.5[A/V] =  0.411311053984

	sp->VoltPerNewton(0.5776f);

	//	１カウントの長さ [m/plus]	実測値: 2.924062107079e-5f
	sp->LenPerPlus(2.924062107079e-5f);

	//	最小張力 [N]
	sp->MinForce(0.5f);

	//	最大張力 [N]
	sp->MaxForce(20.0f);
		@endverbatim
*/
//----------------------------------------------------------------------------------

void main(){

// 初期化部分 -----------------------------------------------------------------------

	DVDeviceManager devMan;		//	デバイスドライバを入れておく入れ物
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0アンプ(ID 10)を登録
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	SH4アンプ(ID 0)を登録
	/*	使っていないドライバ．登録しておけば自動認識する．
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	*/
	// ドライバの初期化．存在するデバイスに対応するバーチャルデバイスがここで初期化される
	devMan.Init();
	std::cout << devMan;	//	デバイスの状態の表示

	HISpidar2 spidar;

	//	SPIDARの初期化．本来は，ハードウエア毎に HISpidar4 クラスを派生させるべき
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.417f	//	x方向の辺の長さ/2
	#define PY	0.0000001f	//	y方向の辺の長さ/2
	#define PZ	0.325f	//	z方向の辺の長さ/2

	Vec3f motorPos[1][4] = {	//	モータの取り付け位置(中心を原点とする)
		Vec3f(PX, -PY, -PZ),Vec3f( PX, PY,PZ),Vec3f(-PX,-PY,PZ),Vec3f( -PX,PY,-PZ)
	};

	spidar.Init(devMan, motorPos[0], 0.41131105f, 2.924062107079e-5f, 1.0f, 20.0f);
// ----------------------------------------------------------------------------------
	
//	マルチメディアタイマーで TimerFunc を呼出 ---------------------------------------	
	WBMMTimer mmtimer;
	mmtimer.Set(TimerFunc, &spidar);
	mmtimer.Resolution(5);
	mmtimer.Interval(5);
	mmtimer.Create();

//	メインループ --------------------------------------------------------------------
	while(1){
		// 描画のための X_object_point Y_object_point をGraphクラスに送る
		X_object_point=coordinate_X_max/2+(int)(coordinate_X_max/2*spidar.GetPos().X()/PX);
		Y_object_point=coordinate_Y_max/2-(int)(coordinate_Y_max/2*spidar.GetPos().Z()/PZ);
		// マウス位置を計算用に変換
		X_M = ((double)X_mouse_pos/coordinate_X_max - 0.5f) * PX * 2;
		Y_M = -1*((double)Y_mouse_pos/coordinate_Y_max - 0.5f) * PZ * 2;
		// 位置指定のためのダイアログ表示
		static Graph p_graph;


		// 目標座標
	//	std::cout << "P_X:" <<spidar.GetPos().X() << "\tP_Y:" <<spidar.GetPos().Z() << std::endl;
		// マウス座標
	//	std::cout << "P_X:" <<X_M    << "\tP_Y:" <<Y_M    << std::endl;
		
		if (kbhit())
		{		//	キー入力
			int ch = getch();
			switch(ch)
			{
			case ' ':		//	スペース：力覚ON/OFF
				bMinForce = !bMinForce;
				break;
			case 'c':		//	キャリブレーション
				spidar.Calib();
				break;
			case 'g':		//	キャリブレーション
					{
						p_graph.Window_Create_Plot();
					break;
					}

			case 0x1b:		//	ESC:終了
				goto end;
				break;
			}
		}	
#if 0
#endif	
	}
// ----------------------------------------------------------------------------------
	// マルチメディアタイマーをストップ
	mmtimer.Release();
end:;
}

//----------------------------------------------------------------------------------
// マルチメディアタイマーで呼び出す関数 
//----------------------------------------------------------------------------------
void TimerFunc(void* arg){
	HISpidar2& spidar = *(HISpidar2*)arg;
		spidar.Update(0.001f);			//	アンプとの通信(力を提示・位置を取得)
		Vec3f force;					//	提示する力
		const float K = 2000.0f;		//	ばね定数
		/*	
		if (spidar.GetPos().Z() < 0){	//	床の提示
			force.Z() = - K * spidar.GetPos().Z();
			std::cout << "P_Z:" << spidar.GetPos().Z() << "\tF_Z:" << spidar.GetForce().Z() << std::endl;

		}
		*/

		//--------------------------------------------------------------------------
		//        速度・加速度を求め，ばね定数・粘性定数をセットする
		//--------------------------------------------------------------------------
		// 現在の位置の差
		static double X_difference = 0;
		static double Y_difference = 0;
		// 現在の速度の差
		static double V_X_difference = 0;
		static double V_Y_difference = 0;
		// 速度の差を決めるための位置の差を残しておく
		double X_difference_buf = X_difference;
		double Y_difference_buf = Y_difference;

		X_difference = X_M - spidar.GetPos().X();
		Y_difference = Y_M - spidar.GetPos().Z();

		V_X_difference = X_difference - X_difference_buf;
		V_Y_difference = Y_difference - Y_difference_buf;

		// ばね定数・粘性定数設定
			force.X()= 100 * spring_modulus * X_difference
				      +100 * damper_modulus * V_X_difference;

			force.Z()= 100 * spring_modulus * Y_difference
				      +100 * damper_modulus * V_Y_difference;
		//--------------------------------------------------------------------------
		if (force.norm() > 10){			//	大きすぎる力は提示できないので，10Nに制約
			force = force.unit() * 10;
		}

		//	提示力の設定
		if (bMinForce) spidar.SetMinForce();
		//else spidar.SetForce(force,0.0000001f);
		else spidar.SetForce(force);

		/*
		static double L0_buf = spidar.motor[0].GetLength();
			   double L0     = spidar.motor[0].GetLength();
			   double L0_difference = L0 - L0_buf;	
			   L0_buf = L0;
		
		spidar.motor[0].SetForce(0.0f);
		spidar.motor[1].SetForce(0.0f);
		spidar.motor[2].SetForce(0.0f);
		spidar.motor[3].SetForce(0.0f);
			 
		//if(spidar.tension[0]<2.0f)	spidar.tension[0] += L0_difference;
		std::cout << "P_0:" << spidar.tension[0] << "\t";
		spidar.motor[0].SetForce(spidar.tension[0] - 500 * L0_difference);

		std::cout << "P_0:" << spidar.tension[0] - 500 * L0_difference<< "\t";
		std::cout << "P_L:" << L0_difference <<std::endl;
		*/
	
	// 粘性補正 --------------------------------------------------------------------
	
		// 現在の位置 グリップの位置
		static double X_G = 0;
		static double Y_G = 0;
		// 現在の速度 グリップの速度
		static double V_X = 0;
		static double V_Y = 0;
		// 速度を決めるための位置を残しておく
		double X_G_buf = X_G;
		double Y_G_buf = Y_G;

		X_G = spidar.GetPos().X();
		Y_G = spidar.GetPos().Z();

		V_X = X_G - X_G_buf;
		V_Y = Y_G - Y_G_buf;

		// グリップから各モータへの位置方向ベクトル
		static double X_D[4] ={0}; 
		static double Y_D[4] ={0}; 
		// 初期化 ---------------------------
			//モータ0 
			X_D[0] = PX - X_G;
			Y_D[0] = Y_G - PZ;
			//モータ1
			X_D[1] = PX - X_G;
			Y_D[1] = PZ - Y_G;
			//モータ2
			X_D[2] = X_G - PX;
			Y_D[2] = PZ - Y_G;
			//モータ3
			X_D[3] = X_G - PX;
			Y_D[3] = Y_G - PZ;
		// ----------------------------------

		// グリップから各モータへの速度方向ベクトル
		static double V_X_D[4] ={0}; 
		static double V_Y_D[4] ={0};
		double P_convention[4];

		double aid_input[4];		//粘性補正分入力
		double aid_input_X[4];		//aid_input のX成分
		double aid_input_Y[4];		//aid_input のY成分


		for(int i=0;i<4;i++)
		{
			P_convention[i] = ( X_D[i] * V_X + Y_D[i] * V_Y ) / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);
			V_X_D[i] =P_convention[i] * X_D[i] / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);
			V_Y_D[i] =P_convention[i] * Y_D[i] / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);

			// モータ回転速度から補正入力への変換
			//aid_input[i] = 50 * P_convention[i] + 0.5f;
			//aid_input[i] = 835.04 * P_convention[i] + 0.258553; //計算したやつ
			//aid_input[i] = 150 * P_convention[i] + 1.0f;		  //ちょうどいいやつ
			aid_input[i] = 0;									  //補正しないやつ
		
			aid_input_X[i] = aid_input[i] * X_D[i] / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);
			aid_input_Y[i] = aid_input[i] * Y_D[i] / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);		
		}
		
		
		int can_not_offset_number[2];	//回転方向により補正できないモータ番号
		for(int i=0;i<4;i++)
		{
			static int j=0;
			if(P_convention[i]<0) //モータの回転方向 マイナス
			{
				can_not_offset_number[j] = i;
				j=(j+1)%2; // 0 or 1を順番に使う
			}
		}
		for(int i=0;i<4;i++)
		{
			if(P_convention[i]>0) //モータの回転方向 プラス
			{
				for(int j=0;j<2;j++)
				{
					int k = can_not_offset_number[j]; //補正できないモータ番号
					aid_input[k] = ( aid_input_X[i] * aid_input_X[k] + aid_input_Y[i] + aid_input_Y[k]) 
									/ sqrt( aid_input_X[i] * aid_input_X[i] + aid_input_Y[i] + aid_input_Y[i]);					
				}
				for(int j=0;j<2;j++)
				{
					//aid_input[i] = aid_input[i] - aid_input[can_not_offset_number[j]];  
				}
				spidar.motor[i].SetForce(spidar.tension[i] + aid_input[i] );

			}
		}
		/*
		int i = 1;	
		static double L0_buf[4]={spidar.motor[0].GetLength(),
								 spidar.motor[1].GetLength(),
								 spidar.motor[2].GetLength(),
								 spidar.motor[3].GetLength()};
		double L0[4];
		double L0_difference[4];
		for(int i=0;i<4;i++)
		{
			L0[i]     = spidar.motor[i].GetLength();
			L0_difference[i] = L0[i] - L0_buf[i];	
			L0_buf[i] = L0[i];

			if(L0_difference<0)
			{
				double aid_input = 835.04 * (-1) * L0_difference[i] + 0.258553;
				spidar.motor[i].SetForce(spidar.tension[i] + aid_input);
			}
		}
		*/

	//	std::cout << "P :" <<  L0_difference[1] <<"\t"<<"\t";		
	//	std::cout << "P :" << V_X_D[i] <<"\t"<<"\t";
	//	std::cout << "P :" << L0_difference + V_X_D[i] <<std::endl;
		std::cout << "P :" <<  spidar.tension[1]  <<std::endl;

}
