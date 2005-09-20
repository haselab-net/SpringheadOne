#ifndef SPIDARINTERFACE_H
#define SPIDARINTERFACE_H

#ifdef __cplusplus
extern "C"{
#endif

///	SPIDARの初期化，他の関数を呼ぶ前に1度呼び出してください．
void SPInit();
void SPCalib();

///	SPIDARの更新．位置を計算して力を提示します．
void SPUpdate();

///	提示力の設定．Update()を呼ぶまでは反映されません．
void SPSetForce(float x, float y, float z);
///	提示トルクの設定．Update()を呼ぶまでは反映されません．
void SPSetTorque(float x, float y, float z);

///	グリップ位置の取得．最後にUpdate()を呼んだときのデータです．
float* SPGetPosition();
///	グリップ位置の取得．最後にUpdate()を呼んだときのデータです．
float* SPGetOrientation();	//	Quaternion形式（float [4]）で返します．
float* SPGetRotation();		//	Matrix形式 (float[9])で返します．

#ifdef __cplusplus
}
#endif


#endif
