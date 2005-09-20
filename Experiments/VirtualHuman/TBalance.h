// TBalance.h: TBalance クラスのインターフェイス
//
//	VHのバランスに必要な計算描画をしてくれる。
//	
//
//////////////////////////////////////////////////////////////////////

#ifndef TBALANCE_H
#define TBALANCE_H

#include <Physics/PHHuman.h>

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include <vector>

#include "THuman.h"
#include "CollisionArea.h"
//#include "QuadricPR.h"

//ZMP計算に必要な重力の定数
#define g_x 0
#define g_y 9.8
#define g_z 0

class TBalance  
{
private:

	//二次計画法
	//QuadricPR quadPR;

	//ZMPの計算
	Spr::Vec3d vh_zmp;
	Spr::Vec3d preVelocityTest;					//zmpの計算に必要なvhの速度
	Spr::Vec3d nowVelocityTest;

	std::vector<Spr::Vec3d> preVelocity;		//zmpの計算に必要なvhの速度
	Spr::Vec3d nowVelocity;
	bool first;									//GetAccelが呼び出されたのが一回目か否か。

	std::vector<Spr::Vec3d> Accel;				//ある時間のVHの各リンクの加速度を持つ

	//ZMPの足域領域チェック
	Spr::Vec3f footPoint[8];			//足裏領域の端点
	Spr::Vec3d footLCenter,footRCenter;	//左右の足の中心の位置
	Spr::Vec3f bBoxMax,bBoxMin;

public:
	TBalance();
	virtual ~TBalance();
	
	//　バランスをチェックに必要なものを全て呼んでくれる。
	void CheckBalance(Spr::GRRender* render, Spr::PHHuman* phHuman, double dt);

	void CalcZmp(Spr::PHHuman* phHuman, double dt);			//VHのZMPを計算する
	void GetAccel(Spr::PHHuman* phHuman, double dt);			//ZMP計算に必要なVHの各リンクの加速度を計算する。
	bool DrawZmp(Spr::PHHuman* phHuman, Spr::GRRender* render);		//ZMPの描画
	Spr::Vec3d GetZmp();									//ZMPを取得
	bool ZmpIncludeCheck();					//ZMPが安定領域内にいるか監視 
	bool PosIncludeTriangle(Spr::Vec3f t_a, Spr::Vec3f t_b, Spr::Vec3f t_c, Spr::Vec3f point);
	
	//安定領域（床との接触領域）の計算クラス
	CollisionArea collisionArea;

	void GetAccelTest(THuman& human, double dt);
	void CalcZmpTest(THuman& human, double dt);
	
	//　使ってない
	void CalcFootPoint(THuman& human);				//足裏領域の計算
	Spr::Vec3f* GetFootPoint();						//足裏領域の端点の取得

	void Reload();									//Reloadされた時の関数

};
#endif
