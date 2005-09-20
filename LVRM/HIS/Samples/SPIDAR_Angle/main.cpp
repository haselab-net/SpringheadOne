#include "HIS/HISpidar4anglequad.h"
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
	((HISpidar4anglequad*)dwUser)->Update();

}


HISpidar4anglequad spidar;
Glut_test gl;
DVDeviceManager devMan;
void main(){

	
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
	//この値は実測値で0,1番目のモータが-y軸方向、2,3番目のモータがy軸方向に取り付けてある場合である
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		Vec3f(-0.310f, 0.210f, 0.245f),	Vec3f( 0.305f, 0.210f,-0.230f), Vec3f(-0.310f,-0.200f,-0.230f), Vec3f( 0.305f,-0.200f, 0.248f)
	};

	//　手元位置の最初の位置、姿勢
	//  (x+x_1,y+y_1,z+z_1),(x-x_1,y-y_1,z-z_1),(x+x_2,y+y_2,z+z_2),(x-x_2,y-y_2,z-+z_2),(x,y,z)
	//  の順番で書いている
	Vec3f initPos[5] = {
		Vec3f(-0.045f,0.0f,0.04f), Vec3f(0.045f,0.0f,-0.04f), Vec3f(-0.045f,0.0f,-0.04f), Vec3f(0.045f,0.0f,0.04f), Vec3f(0.0f,0.0f,0.0f)
	};
/*
	Vec3f initPos[5] = {
		Vec3f(-0.04818f,0.0f,0.04818f), Vec3f(0.04818f,0.0f,-0.04818f), Vec3f(-0.04818f,0.0f,-0.04818f), Vec3f(0.04818f,0.0f,0.04818f), Vec3f(0.0f,0.0f,0.0f)
	};
*/	
	spidar.Init(devMan, motorPos[0], initPos, /*0.2f*/0.3776f, 2.924062107079e-5f, 0.001570796326794896618f/*1.570796326795e-3f*/, 1.5f/*1.0f*/, 20.0f);

//角度用エンコーダの回転の向きを補正するための計算
//	spidar.Motor()[2].anglePerPulse *= -1;
//	spidar.Motor()[3].anglePerPulse *= -1;	


	spidar.Calib();

	//タイマーのスタート
	UINT tid;
	tid = spidar.TimerId = timeSetEvent(1, 1, TimerProc, (DWORD)& spidar, TIME_PERIODIC);

	
	//描画の開始
	gl.Init("SPIDAR ANGLE TEST");
	gl.Run();

	while(1){

/**/
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:	//Esc
				goto end;
				break;
			case  0x36:	//6
				spidar.Calib();
				break;
/**/
			case 'k':
				//spidar.end_flag = 1;
				timeKillEvent(tid);
				//timeKillEvent(spidar.TimerId);
				timeEndPeriod(1);
				break;	
			}
		
		}
//		spidar.Update();
/*		std::cout.width(4);
		std::cout.precision(2);
		std::cout <<  spidar.l1 << " " <<  spidar.l2 << " " <<  spidar.l3 << " " << spidar.l4 << "  ";
		std::cout <<  spidar.a1*(180.0/3.141592) << " " <<  spidar.a2*(180.0/3.141592) <<" "<< spidar.a3*(180.0/3.141592) << " "<<spidar.a4*(180.0/3.141592) <<std::endl;
		std::cout <<  spidar.l1new << " " <<  spidar.l2new << " " <<  spidar.l3new << " " << spidar.l4new << " ";
		std::cout <<  spidar.a1new*(180.0/3.141592) << " " <<  spidar.a2new*(180.0/3.141592) <<" "<< spidar.a3new*(180.0/3.141592) << " "<<spidar.a4new*(180.0/3.141592) <<std::endl;
//		std::cout << std::endl;
*/
//		std::cout << "/****************位置*****************/" << std::endl;
//		std::cout << "X= " <<  spidar.Xnew <<" , " << "Y=   " <<  spidar.Ynew << " , "<< "Z =   " <<  spidar.Znew << std::endl;
//		std::cout << "X1= " <<  spidar.X1new <<" , " << "Y1=   " <<  spidar.Y1new << " , "<< "Z1 =   " <<  spidar.Z1new << std::endl;
//		std::cout << "X2= " <<  spidar.X2new <<" , " << "Y2=   " <<  spidar.Y2new << " , "<< "Z2 =   " <<  spidar.Z2new << std::endl;
//		std::cout << "P1(x)= " <<  spidar.Xnew+spidar.X1new <<" , " << "P1(y)=   " <<  spidar.Ynew+spidar.Y1new << " , "<< "P1(z) =   " <<  spidar.Znew.spidar.Z1new << std::endl;
//		std::cout << "X2= " <<  spidar.X2new <<" , " << "Y2=   " <<  spidar.Y2new << " , "<< "Z2 =   " <<  spidar.Z2new << std::endl;
		

	}

end:;
//データ取得	
/*	spidar.fd = fopen("Xrot.txt", "w");
	fprintf(spidar.fd, "%d\n",spidar.test_time);
	for (int i=0; i<spidar.test_time; i++){
		fprintf(spidar.fd, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", 
			spidar.x_data[i],
			spidar.y_data[i],
			spidar.z_data[i],
			spidar.quatang_data[i],
			spidar.quatvecX_data[i],
			spidar.quatvecY_data[i],
			spidar.quatvecZ_data[i],
			spidar.a1_data[i],
			spidar.a2_data[i],
			spidar.a3_data[i],
			spidar.a4_data[i],
			spidar.l1_data[i],
			spidar.l2_data[i],
			spidar.l3_data[i],
			spidar.l4_data[i] 
			);
	}
	fclose(spidar.fd);

*/

}


