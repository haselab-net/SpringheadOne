#include "HIS/HISpidar4AngleAxisQuad.h"
#include "glut_test.h"
/// USB20を使わないときには下の「#define _USB20_」をコメントアウト
//#define _USB20_

#ifdef _USB20_
#include <Device/DRUsb20Simple.h>
#else
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <Device/DRUsbH8Simple.h>
#endif //_USB20_

#include <conio.h>
#include <iostream>
//追加
#include<GL/glut.h>
#include <windows.h>
#include <mmsystem.h>

const float K = 2000;
using namespace LVRM;

//タイマー用の変数
	DWORD uTimerRes;
	UINT TimerId;

//Multimediatimer
//ここではタイマーで行う処理を書く
void CALLBACK TimerProc(
    UINT uID,     // タイマーID
    UINT uMsg,    // 使わない
    DWORD dwUser, // ユーザー定義
    DWORD dw1,    // 使わない
    DWORD dw2     // 使わない
   )
{
	//タイマー処理
	((HISpidar4angleaxisquad*)dwUser)->Update();

}


HISpidar4angleaxisquad spidar;
Glut_test gl;
void main(){

	DVDeviceManager devMan;
	
	//Glut_testで作ったHISpidar4quadのオブジェクトにアドレスをいれる
	gl.sp = &spidar;

#ifdef _USB20_
	devMan.RPool().Register(new DRUsb20Simple(0));
#else
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
#endif //_USB20_
	devMan.Init();

	std::cout << devMan;

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
//	#define PX	0.265f//0.53f	//	x方向の辺の長さ/2
//	#define PY	0.265f//0.53f	//	y方向の辺の長さ/2
//	#define PZ	0.265f//0.53f	//	z方向の辺の長さ/2
	//この値は実測値で0,3番目のモータが-z軸方向、1,2番目のモータが-y軸方向に取り付けてある場合である
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		Vec3f(-0.315f, 0.280f, 0.172f),	Vec3f( 0.315f, 0.200f,-0.230f), Vec3f(-0.315f,-0.200f,-0.230f), Vec3f( 0.315f,-0.280f, 0.172f)
	};

	

	//　手元位置の最初の位置、姿勢
	//  (x+x_1,y+y_1,z+z_1),(x-x_1,y-y_1,z-z_1),(x+x_2,y+y_2,z+z_2),(x-x_2,y-y_2,z-+z_2),(x,y,z)
	//  の順番で書いている
	Vec3f initPos[5] = {
		Vec3f(-0.045f,0.0f,0.04f), Vec3f(0.045f,0.0f,-0.04f), Vec3f(-0.045f,0.0f,-0.04f), Vec3f(0.045f,0.0f,0.04f), Vec3f(0.0f,0.0f,0.0f)
	};
	//CDバージョン
/*	Vec3f initPos[5] = {
		Vec3f(-0.0389f,0.0f,0.0389f), Vec3f(0.0389f,0.0f,-0.0389f), Vec3f(-0.0389f,0.0f,-0.0389f), Vec3f(0.0389f,0.0f,0.0389f), Vec3f(0.0f,0.0f,0.0f)
	};
*/	
	spidar.Init(devMan, motorPos[0], initPos, /*0.2f*/0.3776f, 2.924062107079e-5f, 0.001570796326794896618f/*1.570796326795e-3f*/, 2.0f, 20.0f);
	//角度用エンコーダの回転の向きを補正するための計算
	spidar.Motor()[0].anglePerPulse *= -1;
	spidar.Motor()[2].anglePerPulse *= -1;
	spidar.Motor()[3].anglePerPulse *= -1;
	//モータの回転を逆にする場合
//	spidar.Motor()[0].lengthPerPulse *= -1;

	spidar.Calib();

	//タイマーのスタート
//	TimerId = timeSetEvent(1, 1, TimerProc, (DWORD)& spidar, TIME_PERIODIC);

	
	//描画の開始
//	gl.Init("SPIDAR ANGLE TEST");
//	gl.Run();

	while(1){

		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				goto end;
				break;
			case  0x36:
				spidar.Calib();
				break;
			}
		}




//		std::cout << "/****************長さと角度*****************/" << std::endl;
/*		std::cout << "l1   =   " <<  spidar.l1 << std::endl;
		std::cout << "l2   =   " <<  spidar.l2 << std::endl;
		std::cout << "l3   =   " <<  spidar.l3 << std::endl;
		std::cout << "l4   =   " <<  spidar.l4 << std::endl;
		std::cout << "a1   =   " <<  spidar.a1*(180.0/3.141592) << std::endl;
		std::cout << "a2   =   " <<  spidar.a2*(180.0/3.141592) << std::endl;
		std::cout << "a3   =   " <<  spidar.a3*(180.0/3.141592) << std::endl;
		std::cout << "a4   =   " <<  spidar.a4*(180.0/3.141592) << std::endl;
		std::cout << initPos[0].Z()-motorPos[0][0].Z() <<std::endl;
		std::cout << initPos[0].X()-motorPos[0][0].X() <<std::endl;
		std::cout << atan(-0.225/0.22)*(180.0/3.141592) <<std::endl;
		std::cout << std::endl;
*/
//		std::cout << "/****************位置*****************/" << std::endl;
//		std::cout << "X= " <<  spidar.Xnew <<" , " << "Y=   " <<  spidar.Ynew << " , "<< "Z =   " <<  spidar.Znew << std::endl;
//		std::cout << "X1= " <<  spidar.X1new <<" , " << "Y1=   " <<  spidar.Y1new << " , "<< "Z1 =   " <<  spidar.Z1new << std::endl;
//		std::cout << "X2= " <<  spidar.X2new <<" , " << "Y2=   " <<  spidar.Y2new << " , "<< "Z2 =   " <<  spidar.Z2new << std::endl;
//		std::cout << "P1(x)= " <<  spidar.Xnew+spidar.X1new <<" , " << "P1(y)=   " <<  spidar.Ynew+spidar.Y1new << " , "<< "P1(z) =   " <<  spidar.Znew.spidar.Z1new << std::endl;
//		std::cout << "X2= " <<  spidar.X2new <<" , " << "Y2=   " <<  spidar.Y2new << " , "<< "Z2 =   " <<  spidar.Z2new << std::endl;
		
		spidar.Update();
/*
		float rad = 0.25;
		Vec3f colde;
		Vec3f force; 

		Affinef graphic = Affinef(0, 0, -0.5);

		colde = graphic.Pos() - spidar.GetPos(4); 
						
		spidar.Update();
		if (rad - colde.norm() > 0){
			force = -K * (rad - colde.norm()) * (graphic.Pos() - spidar.GetPos(4)).unit();
			}
			spidar.SetForce(force, 0.1f);
*/
	}


end:;
}


