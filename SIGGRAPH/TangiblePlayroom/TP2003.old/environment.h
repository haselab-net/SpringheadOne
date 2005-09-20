#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <Base/VRBase.h> //Vec3fのためだけに必要
#include "GameView.h" 

//SPIDARタイプを整数定義
#define Env_NO_SPIDAR		0
#define Env_ISA_BIG_SPIDAR	1
#define Env_USB_SPIDAR_G	2

//こんなところに書いて悪いけどSPRITEの枚数
#define MAX_SPRITES 9
#define MAX_LANGUAGES 5

#define MOUSE_SAMPLE 100 //マウスの平均フレーム数
// 描画が遅い環境のため
//#define PH_GRAPHIC_LOW_DETAIL

using namespace LVRM;
/*
 environment.h
 実験環境設定クラス
*/

//実験環境を定義するクラス
class Environment {
public:
	Environment(); //コンストラクタ
	const char * filename; //環境設定ファイルの名前
	bool Load(); //ファイルロード

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
	bool bRoll90;      //プロジェクタ方向90度回転モード
	bool bHaptics;     //力覚モード

	//本当はここにあってほしくない記録用データ領域
	Vec3f dForce; //記録時間内のSPIDAR出力の総和
	float dRatio;

	GameView	*view; //いつでもどこでもアクセスできるようにするためだけに

	int iStaticFrame; //Grasp状態を判断するための、SPIDAR入力値が同じフレームの数
	int iGraspFrame;  //同じく把持状態のフレーム数
	Vec3f	vSpidar;  //SPIDAR生座標
	Vec3f	vSpidarLast;  //SPIDAR座標,ひとつまえ
	Vec3f   vSpidarForce; //SPIDAR指令値
	Vec3f   vSpidarForceLast; //SPIDAR指令値,ひとつまえ
	Vec3f	viewscale; //映像とSPIDARの座標あわせ


	//安全対策
	bool	bEmergency; //非常ブレーキ判定

	//多言語対応
	int		iLanguage;  //言語ID{日本語,英語,...}

	//マウス
	int mousex[MOUSE_SAMPLE];
	int mousey[MOUSE_SAMPLE];
	int mousez[MOUSE_SAMPLE];
	Vec3f v3DMouse;  //仮想三次元マウス

	bool bTallNone; //高さ無視モード

};


extern Environment env;
#endif
