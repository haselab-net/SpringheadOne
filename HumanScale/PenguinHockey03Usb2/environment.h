#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Base/VRBase.h> //Vec3fのためだけに必要

//SPIDARタイプを整数定義
#define Env_NO_SPIDAR		0
#define Env_ISA_BIG_SPIDAR	1
#define Env_USB_SPIDAR_G	2

using namespace LVRM;
/*
 environment.h
 実験環境設定クラス
*/

//実験環境を定義するクラス
class Environment {
public:
	const char * filename; //環境設定ファイルの名前
	bool Load();

	//以下設定ファイルの記述順と同じ。
	int idClient; //クライアントを定義するID、ゼロはサーバ
	char nameServer[255];	//接続するサーバのIPもしくは名前
	bool bUseSound;	//サウンドを使用する場合"UseSound"を指定する
	bool bUseGraphics;	//グラフィックスを使用する場合"UseGraphics"を指定する
	int iSpidar_type; //使用したいSPIDARのタイプ詳細は上でdefineしてます
	//以下、過去のソースとの互換のために存在。
	bool bClient; //クライアントならtrue,idClientから設定。
	bool bSpidarExist; //SPIDARが実在するならtrue
	float	px,py,pz;	//SPIDARが取り付けられている辺の長さ/2
	int	idExperiment;	//実験ID、ゼロはゲームモード

	bool bFullscreen;  //フルスクリーン描画
	bool bHaptics;     //力覚モード

	//本当はここにあってほしくない記録用データ領域
	Vec3f dForce; //記録時間内のSPIDAR出力の総和
	float dRatio;

};


extern Environment env;
#endif
