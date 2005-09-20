/***********************************************************************/
/*                                                                     */
/*  FILE        :qp_prog01.h                                           */
/*  DATE        :2002/01/29                                            */
/*  DESCRIPTION :Quadratic Programming (Active Set Method)             */
/*                                                                     */
/*  Katsuhito AKAHANE                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef _QP_PROG01_H_
#define _QP_PROG01_H_

//
// f(x) = (1/2)x'Qx - c'x    ->minimize
// minX <= x <= maxX
//


// 注意！！！
// 使う前に、次元の設定を忘れないこと！！！
#define N 4					//二次計画の次元数

extern float gVecX[N];		//out:最適解

extern float gMatQ[N][N];	//in:二次計画の２次の係数
extern float gVecC[N];		//in:二次計画の１次の係数

//
// 二次計画法の初期化処理関数
// ボックス条件の設定 (minTin <= X <= maxTin)
extern void InitQuadpr(float minTin, float maxTin);

//
// 二次計画法メイン関数
// この関数を呼び出す前に
// InitQuadpr() を呼び出し
// gMatQ[N][N]
// gVecC[N]
// の設定をすること
// また、gMatQ,gVecC は、計算の前後で変更されることはない。
extern int quadpr();// 初期探索点を前回の解にする
extern int quadpr2();// 初期探索点を無条件最適解の近傍にする

#endif //_QP_PROG01_H_

