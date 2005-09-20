#ifndef SETTINGS_H
#define SETTINGS_H
/*

 setting.h
 ゲームの初期値などを設定する
 [*]でコメントされている値はシーンデザイン変更時、衝突関係で変更が必要な値
*/

#define NUM_PUCK	4	//Puckの数

//開始位置
const float	lydown = 0.45f;		//原点から床平面までの距離
const float ly = 0.75f;       //〃天井平面までの距離
const float lx = 0.65f;		//〃両端までの距離
//const float lzfront = 0.55f;		//〃前面(長辺)までの距離[*]
const float lzfront = 0.90f;		//〃前面(長辺)までの距離
//const float lzback = 0.75f;		//〃後面までの距離[*]
const float lzback = 0.0f;		//〃後面までの距離

const float lyResetb = 0.65f;		//ペンギン(胴体)が当たり判定を始める境界面
const float lyReseth = 0.68f;		//ペンギン(頭部)が当たり判定を始める境界面

//debug
// InitPenguinPos, startPenX あたりの定数と統一すべき？
const Vec3f InitPenguinPos[] = {
	( 0.40f,-10.0*lydown+0.05, 0.15f),
	(-0.40f,-10.0*lydown+0.05,-1.00f),
	( 0.40f,-10.0*lydown+0.05,-1.50f)
};

const Vec3f startPen1( 0.40f,-1.0*lydown+0.05, 0.15f);	//Penguin1の初期位置
const Vec3f startPen2(-0.40f,-1.0*lydown+0.05,-1.00f);	//Penguin2の初期位置
const Vec3f startPen3( 0.40f,-1.0*lydown+0.05,-1.50f);	//Penguin3の初期位置

/*
const Vec3f InitPuckPos[] = {
	(-0.15f,-1.0f*lydown,-0.50f),
	( 0.15f,-1.0f*lydown,-0.50f),
	( 0.15f,-1.0f*lydown,-0.80f),
	(-0.15f,-1.0f*lydown,-0.80f)
};
*/

const Vec3f InitPuckPos[] = {
	(-0.15f,-1.0f*lydown,-0.15f),
	(-0.15f,-1.0f*lydown, 0.15f),
	( 0.15f,-1.0f*lydown,-0.15f),
	( 0.15f,-1.0f*lydown, 0.15f)
};

/*
const Vec3f startPu1(-0.15f,-1.0f*lydown,-0.50f);	//Puck1の初期位置
const Vec3f startPu2( 0.15f,-1.0f*lydown,-0.50f);	//Puck2の初期位置
const Vec3f startPu3( 0.15f,-1.0f*lydown,-0.80f);	//Puck3の初期位置
const Vec3f startPu4(-0.15f,-1.0f*lydown,-0.80f);	//Puck4の初期位置
*/


const Vec3f startPu1(-0.15f,-1.0f*lydown,-0.15f);	//Puck1の初期位置
const Vec3f startPu2(-0.15f,-1.0f*lydown, 0.15f);	//Puck2の初期位置
const Vec3f startPu3( 0.15f,-1.0f*lydown,-0.15f);	//Puck3の初期位置
const Vec3f startPu4( 0.15f,-1.0f*lydown, 0.15f);	//Puck4の初期位置


//const float goalr = 0.30f;		//ゴールの半径[*]
const float goalr = 0.20f;		//ゴールの半径

const float PenheadR = 0.10f;		//ペンギンの頭半径
const float PenbodyR = 0.15f;		//ペンギンの胴体半径
const float Penmass = 1.5f;			//ペンギンの重量
const Vec3f PenheadPos = Vec3f(0,0.33,0);		//ペンギンの頭座標  底が原点
const Vec3f PenbodyPos = Vec3f(0,0.15,0);		//ペンギンの胴体座標

const float PuckheadR = 0.07f;		//Puck頭部半径
const float PuckbodyR = 0.10f;		//Puck胴体部半径


// サウンド関連の定数値
#define	MAXSNDPARAM			4.0	// グローバル変数の最大値を指定(正規化に使用)

#endif