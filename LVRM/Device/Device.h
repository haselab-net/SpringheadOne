#ifndef LVRM_DEVICE_H
#define LVRM_DEVICE_H
/**	@mainpage LVRM Device ライブラリ
@section aim 目的
	このパッケージは，デバイスドライバを抽象化するためのレイヤーです．アプリケーションからデバイスドライバを直接呼び出すと，デバイスを変更したときに，アプリケーションを修正しなければならなくなります．そこで，例えばD/AコンバータならどのD/Aコンバータでも同じように呼び出せるように抽象化したD/Aコンバータデバイス(バーチャルデバイス)を考え，アプリケーションはバーチャルデバイスを使用するようにします．バーチャルデバイスは実際のデバイス(リアルデバイス)を呼び出すように実装してあります．

@section usage 使い方
簡単なサンプルを示します．
@verbatim
	DVDeviceManager devMan;				//	デバイス管理クラス．
	devMan.RPool().Register(new DRAdapio(9));	//	リアルデバイスの登録
	devMan.Init();						//	デバイスの初期化，ここで
										//	バーチャルデバイスが生成される．
	std::cout << devMan;				//	状態の表示
	
	//	デバイス管理クラスからA/Dを借りてくる
	DVAdBase* ad;
	devMan.VPool().Rent(ad);
	std::cout << ad->Voltage();
@endverbatim
まずデバイスマネージャを作成し，実デバイスを登録し，初期化します．するとバーチャルデバイスが生成されるので，アプリケーションは，使いたいバーチャルデバイスをデバイスマネージャから借りてきます．

特定のデバイスの特定の機能を使いたい場合，専用のデバイスマネージャを作成して，デバイスを登録，初期化して，バーチャルデバイスを取得してください．

デバイスマネージャは複数作成しても問題ありません．
*/

#include "DVDeviceManager.h"

#include "DRContecIsaCounter.h"
#include "DRContecIsaDa.h"
#include "DRNittaForce.h"
#include "DRUsbH8Simple.h"

#endif
