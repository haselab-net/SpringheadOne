#include "His/HISpidar4.h"
//	USB2.0アンプのドライバ
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
//	ContectのISAカードのドライバ．古いもの(1997-)．
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>

#include <conio.h>
#include <iostream>

using namespace Spr;	//	SprはSpringheadライブラリの名前空間

void main(){
	//------------------------------------------------------------------------
	//	初期化部分
	//
	DVDeviceManager devMan;		//	デバイスドライバを入れておく入れ物
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0アンプ(ID 10)を登録
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	SH4アンプ(ID 0)を登録
	/*	使っていないドライバ．登録しておけば自動認識する．
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	*/
	/*	ドライバの初期化．存在するデバイスに対応するバーチャルデバイスが
		ここで初期化される	*/
	devMan.Init();
	std::cout << devMan;	//	デバイスの状態の表示

	HISpidar4 spidar;
	HISpidar4 spidar2;

	//	SPIDARの初期化．本来は，ハードウエア毎に HISpidar4 クラスを派生させるべき
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

		// 2003.10 NEW SETTING
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};*/

	spidar.Init(devMan, motorPos[0], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
	spidar2.Init(devMan, motorPos[1], 0.3776f, -2.924062107079e-5f, 0.5f, 20.0f);
	
	

	//------------------------------------------------------------------------
	//	メインループ
	//
	bool bMinForce = true;	//	いきなり力を出すと危険なので，スペースを押すまでは最小張力を出力
	while(1){
		if (kbhit()){		//	キー入力
			int ch = getch();
			switch(ch){
			case ' ':		//	スペース：力覚ON/OFF
				bMinForce = !bMinForce;
				break;
			case 0x1b:		//	ESC:終了
				goto end;
				break;
			}
		}
		//------------------------------------------------------------------------
//			static float pos_x_buf1=0,pos_x_buf2=0,pos_x_buf3=0;

		//------------------------------------------------------------------------

		spidar.Update(0.001f);				//	アンプとの通信(力を提示・位置を取得)
		Vec3f force;					//	提示する力
		const float K = 20.0f;		//	ばね定数 2000->20
		if (spidar.GetPos().Y() < 0){	//	床の提示
			force.Y() = - K * spidar.GetPos().Y();
		}
		if (force.norm() > 3){			//	大きすぎる力は提示できないので，3N(10N)に制約
			force = force.unit() * 3;
		}
		//------------------------------------------------------------------------
/*			pos_x_buf3=pos_x_buf2;
			pos_x_buf2=pos_x_buf1;
			pos_x_buf1=spidar.GetPos().X();
			std::cout << "1:" << pos_x_buf1 << "\t2:" << pos_x_buf2 << "\t3:" << pos_x_buf3 << std::endl;

			force.X() = -5.0f*pos_x_buf2-pos_x_buf1-pos_x_buf3;
*/		//------------------------------------------------------------------------
		//	提示力の設定
		if (bMinForce) spidar.SetMinForce();
		else spidar.SetForce(force);

		//	以下2番目のSPIDARについて同様に，
		Vec3f force2;
		spidar2.Update(1); //1khz 0.001f

		if (spidar2.GetPos().Y() < 0){
			force2.Y() = - K * spidar2.GetPos().Y();
		}
		if (bMinForce) spidar2.SetMinForce();
		else spidar2.SetForce(force2);
		

	
		//	デバッグ用：位置と力を表示
		std::cout << "P:" << spidar.GetPos() << "\tF:" << spidar.GetForce() << std::endl;

		//	デバッグ用：糸の長さを表示
/*
		for(int i=0; i<4; i++){
			std::cout << spidar.Motor()[i].GetLength() << " ";
		}
		for(int i=0; i<4; i++){
			std::cout << spidar2.Motor()[i].GetLength() << " ";
		}
		std::cout << std::endl;
*/
} 
end:;
}
