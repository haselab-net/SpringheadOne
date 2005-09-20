// QuadricPR.cpp: QuadricPR クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuadricPR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

QuadricPR::QuadricPR()
{
	input[4] = 0;			//0:minの最適化問題、1:max
	input[6] = 1000;		//ピボットの最大回数
	input[7]=1;				//0：目的関数を計算しない　1:最適解が見つかったとき、目的関数の値を計算する
	input[8]=0;				//0：問題のパラメータを出力しない　1：する
	input[9]=0;				//0：
	input[10]=0;			//0：ピボットの回数を出力する　1：しない
	input[11]=0;			
	input[12]=0;
	input[13]=80;
	
	tol[0] = tol[1] = 0;
	strcpy(title, "Quadratic Ploblem");
	strcpy(pfile, "quad_savefile");
	title_len = 0;
	pfile_len = 0;
}

QuadricPR::~QuadricPR()
{

}



void QuadricPR::CalcQuadPR(THuman& human, int numConst, int numVari, bool flagEq){
	//numConst:制約条件式の数(M)、numVari:変数の数(N)
	//flagEq:制約条件式に不等号があるかないか。
	
	human.joints.size();
	///// 各パラメータの動的メモリ確保 /////
	//入力
	Quad = new float[numConst*numVari];
	cost = new float[numVari];
	A = new float[numConst*numVari];
	rhs = new float[numConst];
	kt = new long int[numConst];

	//出力先
	solution = new float[numVari];
	rc = new float[numVari];
	dual = new float[numVari];
	slk = new float[numConst];
	
	if(flagEq)
		workSpace = new float[2*(numConst+numVari)^2+12*(numConst+numVari)+16];
	else
		workSpace = new float[2*(numConst+numVari)^2+8*(numConst+numVari)+6];

	
	////// 各パラメータの設定 //////

	for(int i = 0; i < numConst; ++i)
		kt[i] = 0;

	if(numConst > numVari)
		input[0] = numVari;
	else
		input[0] = numConst;

	input[1] = numVari;
	input[2] = numConst;
	input[3] = numVari;
	input[5] = (int)workSpace;

	//Quad & A
	for(int i = 0; i < numConst; ++i){
		for(int j = 0; j < numVari; ++j){
			//cost[j] = -2*
			if(i ==j)
				Quad[i*numVari + j] = 1.0;
			else
				Quad[i*numVari + j] = 0.0;
		}
	}

	//cost
	for(int time = 0; time < numVari/human.joints.size(); ++time){
		for(int jointNum = 0; jointNum < human.joints.size(); ++jointNum){
			cost[time*(numVari/human.joints.size())+jointNum] = -2*jointOpt[time][jointNum];
		}
	}

	//A:未完成

	//rhs:未完成


	//二次計画法を解く関数 quadpr_
	quadpr_(A, kt, rhs, cost, Quad, input, tol, title, pfile,
		obj, solution, rc, dual,slk,iout, workSpace, title_len, pfile_len);

	//今後deleteするものをちゃんとチェックする。場所も(デストラクタ）
	delete [] Quad, A, cost,rhs,kt,solution,rc,dual,slk,workSpace;

}