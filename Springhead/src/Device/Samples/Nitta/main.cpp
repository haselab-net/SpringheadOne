#include <Device/DVDeviceManager.h>
#include <Device/DRNittaForce.h>
#include <conio.h>
#include <iostream>

using namespace Spr;

void main(){
	
	DVDeviceManager devMan;	//	デバイス管理するクラス．
	
	//	デバイス管理クラスにデバイスを登録
	
#if 0	//	IOポートを直接アクセスする場合：
	//	ここでボードのアドレスを設定(マニュアルを見て直してください)
	devMan.RPool().Register(new DRNittaForce(0x230));
#else	//	jr3.dll を使う場合．ボード番号を指定する．
	devMan.RPool().Register(new DRNittaForce(0));
#endif
	//	デバイスの初期化と結果の表示
	devMan.Init();
	std::cout << devMan;
	
	//	デバイスが提供する機能(Virtual Device)から，力センサを探す
//	DVForceBase* sensor = (DVForceBase*)devMan.VPool().Rent("Force");
	DVForceBase* sensor;
	devMan.VPool().Rent(sensor);
	//	見つかったセンサのうち最初の物を使用．
	if (sensor){
		while(!kbhit()){
			Vec3f f,t;	//	力とトルク
			sensor->GetForce6(f,t);
			std::cout << f << t << std::endl;
		}
		getch();
	}
}
