#ifndef HISPIDARCALCBASE_H
#define HISPIDARCALCBASE_H

#include <Base/BaseDebug.h>
#include <Base/Affine.h>
#include "qp_prog01.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <HIS/Fortran/quadpr.h>

namespace Spr{;

class   HISpidarCalcBase{
protected:
	PTM::VVector<float> l_en;				///<	エンコーダ値から得られるワイヤ長さ
	PTM::VVector<float> l_calc;				///<	前の姿勢から計算されるワイヤ長さ
	PTM::VMatrix<float> WireMat;			///<	連立方程式　WireMat x = VecL
	PTM::VMatrix<float> matATA;				///<	連立方程式の係数行列
	PTM::VVector<float> VecL;				///<	連立方程式
	PTM::VVector<float> ATVecL;				///<	AT*VecL
	PTM::VVector<float> se_out;				///<	連立方程式の解
	std::vector<Vec3f>  vec_t;				///<	ワイヤ方向単位ベクトル（並進）
	std::vector<Vec3f>  vec_r;				///<	ワイヤ方向単位ベクトル（回転モーメント）

	///@name fortrun 二次計画法用変数
	//@{
	PTM::VVector<float> q;
	PTM::VVector<float> cost;
	PTM::VVector<float> ca, rhs, Tq, rc, dual, slk;
	PTM::VVector<long> kt;
	PTM::VVector<float> Tension;
	//@}
	
	//デバイス設計者が設定する数
	PTM::VVector<float> minForce;				///<	最小張力
	PTM::VVector<float> maxForce;				///<	最大張力

public:
	HISpidarCalcBase(){}						///<	コンストラクタ
	virtual ~HISpidarCalcBase(){}				///<	デストラクタ
	virtual void Update()=0;					///<	姿勢情報の更新

protected:
	virtual void SetSize(int dof, int nWire);	///<	次元とワイヤの本数に応じたメモリの確保

	virtual void MakeWireVec()=0;				///<	ワイヤ方向ベクトルの計算
	virtual void UpdatePos()=0;					///<	ワイヤ設置位置座標の更新
	virtual void MeasureWire()=0;				///<	ワイヤ長の計測
};

}	//	namespace Spr

#endif
