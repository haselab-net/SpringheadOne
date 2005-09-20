#ifndef VR_HIS_H
#define VR_HIS_H

/**	@page HIS Human Interface System ライブラリ

@section HISAim 目的
	このパッケージは，ヒューマンインタフェースシステムを動かすためのソフトウエア
のライブラリです．アプリケーションは，このパッケージのクラスを利用します．

@section HISStructure 構成
@subsection HISSpidar SPIDAR
SPIDARを動かすためのソフトウエアのライブラリです．
4本糸SPIDAR用 Spr::HISpidar4, SPIDAR-G用 Spr::HISpidarG6 などのクラスがあるほか，
特定のSPIDAR（SPIDAR-G CType用） Spr::HISpidarG6C のようなクラスがあります．
新しいSPIDARを作ったら，そのデバイス用にクラスをひとつ作るようにしてください．

@subsection HISMOVE 移動入力インタフェース
TBW

*/

#ifdef _WIN32
 #include <WinBasis/WBWin.h>
 #include <mmsystem.h>
#endif
#include <Device/DRRealDevice.h>
#include "HIBase.h"
#include "HIHapticDevice.h"
#include "HISpidarCalc3DOF.h"
#include "HISPidarCalc6DOF.h"
#include "HISpidarCalcBase.h"
#include "HISpidarMotor.h"
#include <fstream>

#endif
