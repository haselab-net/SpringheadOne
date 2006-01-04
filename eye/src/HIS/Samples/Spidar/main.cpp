#include <iostream>
#include "His/HISpidar4.h"


#ifdef _WIN32
 #include <windows.h>
 #include <conio.h>

 //	USB2.0アンプのドライバ
 #include <Device/DRUsb20Simple.h>
 #include <Device/DRUsb20Sh4.h>
 //	ContectのISAカードのドライバ．古いもの(1997-)．
 //#include "Device/DRContecIsaDa.h"
 //#include "Device/DRContecIsaCounter.h"
 //#include <Device/DRUsbH8Simple.h>
#endif

#ifdef __sh__
 #include <cyg/kernel/kapi.h>
 #include <Device/DRIHCSh4.h>
 int kbhit(){return false;}
 int getch(){ return 0;}
 void Sleep(int msec){
	cyg_thread_delay(msec*10);
 }
 #define cout DSTR
#endif

using namespace Spr;	//	SprはSpringheadライブラリの名前空間
using namespace std;

int main(){
	//------------------------------------------------------------------------
	//	初期化部分
	//
	DVDeviceManager devMan;		//	デバイスドライバを入れておく入れ物

#ifdef _WIN32
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0アンプ(ID 10)を登録
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	SH4アンプ(ID 0)を登録
	//	IDはアンプ裏のロータリスイッチの値(0x0-0xF)です．

	/*	使っていないドライバ．登録しておけば自動認識する．
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	*/
	bool bMinForce = true;							//	いきなり力を出すと危険なので，スペースを押すまでは最小張力を出力
	const float K = 2000.0f;						//	ばね定数
#endif
#ifdef __sh__
	devMan.RPool().Register(new DRIHCSh4);			//	SH4用のRealDeviceを登録
	bool bMinForce = false;							//	SHの場合は通信できないので，最初からON
	const float K = 20000.0f;						//	ばね定数 10kHzだから10倍
#endif

	/*	ドライバの初期化．存在するデバイスに対応するバーチャルデバイスが
		ここで初期化される	*/
	devMan.Init();
	cout << devMan;	//	デバイスの状態の表示

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
	cout.flush();
	spidar.Init(devMan, motorPos[0], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
	spidar2.Init(devMan, motorPos[1], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
	cout.flush();
	printf("Waiting...");
	Sleep(1000);
	printf("Calibration\n");
/*
	spidar.Motor()[0].lengthPerPulse *= -1;
	spidar2.Motor()[0].lengthPerPulse *= -1;
	spidar2.Motor()[2].lengthPerPulse *= -1;
*/
	spidar.Calib();
	spidar2.Calib();
	
	//------------------------------------------------------------------------
	//	メインループ
	//
	while(1){
		if (kbhit()){	//	キー入力
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
		
		spidar.Update(0.001f);				//	アンプとの通信(力を提示・位置を取得)
		Vec3f force;					//	提示する力
		if (spidar.GetPos().Y() < 0){	//	床の提示
			force.Y() = - K * spidar.GetPos().Y();
		}
		if (force.norm() > 10){			//	大きすぎる力は提示できないので，10Nに制約
			force = force.unit() * 10;
		}
		//------------------------------------------------------------------------
		//	提示力の設定
		if (bMinForce) spidar.SetMinForce();
		else spidar.SetForce(force);

		//	以下2番目のSPIDARについて同様に，
		Vec3f force2;
		spidar2.Update(0.001f);
		if (spidar2.GetPos().Y() < 0){
			force2.Y() = - K * spidar2.GetPos().Y();
		}
		if (bMinForce) spidar2.SetMinForce();
		else spidar2.SetForce(force2);
		//	デバッグ用：位置と力を表示
#if 0
		cout << "P:" << spidar.GetPos() << "\tF:" << spidar.GetForce() << std::endl;
#endif
		//	デバッグ用：糸の長さを表示
#if 1
		for(int i=0; i<4; i++){
			cout << spidar.Motor()[i].GetLength() << " ";
		}
		for(int i=0; i<4; i++){
			cout << spidar2.Motor()[i].GetLength() << " ";
		}
		cout << std::endl;
		cout.flush();
#endif
	} 
end:;
	return 0;
}
