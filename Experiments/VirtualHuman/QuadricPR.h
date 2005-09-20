// QuadricPR.h: QuadricPR クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#ifndef QUADRICPR_H
#define QUADRICPR_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include <vector>
#include "THuman.h"
#include "quadpr.h"		//二次計画法に必要なヘッダー（中身の宣言コピペでもＯＫ）

class QuadricPR  
{
private:
	//QuadraticPR
	//静的
	//入力
	long int input[14];		//各設定変数（必ず14個）
	float tol[2];			//許容誤差（必ず２個）
	char title[18];			//問題のタイトル
	char pfile[14];			//出力先のファイルの名前
	int title_len;
	int pfile_len;
	
	//出力先
	float obj[1];			//目的関数の値（最適解が見つかった時の）
	long int iout[2];		//計算の結果を表すパラメータ（必ず２個）

	//動的（newで後に確保する）
	//二次元配列のものでもquadpr()が一次元でしか受け付けていないので一次元にしている
	//入力
	float* Quad;			//目的関数の二次係数ベクトル（Ｍ×Ｎ）
	float* cost;			//目的関数の一次係数ベクトル（Ｎ）
	float* A;				//制約条件の左辺の係数ベクトル（Ｍ×Ｎ）
	float* rhs;				//制約条件の右辺の係数ベクトル（Ｍ）
	long int* kt;			//制約条件の不等号（Ｍ）(-1:>=,0:=,1:<=)

	//出力先
	float *solution;		//最適解（Ｎ）
	float *rc;				//the reduced cost voctor(N)
	float *dual;			//dual solution vecotr(N)
	float *slk;				//スラックス変数のベクトル（Ｍ）
	
	float *workSpace;		//ワークスペース（サイズは、制約条件の式に等号がある場合：2*(M+N)^2+12*(M+N)+16
							//							制約条件の式に等号がない場合：2*(M+N)+8*(M+N)+6

	std::vector<float>* jointOpt;

public:
	QuadricPR();
	virtual ~QuadricPR();
	void CalcQuadPR(THuman& human, int numConst, int numVari, bool flagEq);	//二次計画問題を解く
	//numConst:制約条件式の数(M)、numVari:変数の数(N)

};

#endif // !defined(AFX_QUADRICPR_H__F6084177_9D43_4B10_8028_E44F126CFC0E__INCLUDED_)
