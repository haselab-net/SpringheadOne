// MoveFloor.h: CMoveFloor クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEFLOOR_H__91F5DF09_A733_47F4_834D_BFD580470BF5__INCLUDED_)
#define AFX_MOVEFLOOR_H__91F5DF09_A733_47F4_834D_BFD580470BF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<GL/glut.h>
#include<stdio.h>

namespace vroadrunner{

#define F_SIZE 6
#define V_SIZE 8
#define E_SIZE 12

class CMoveFloor  
{
public:
	GLfloat vertex[V_SIZE][3];
	int static edge[][2];
	int static face[][4];
	GLdouble static normal[][3];

	//動く床の場所(始点の位置）
	GLfloat x;
	GLfloat y;
	GLfloat z;

	//動く床の大きさ
	GLfloat w;
	GLfloat d;
	GLfloat h;
	
	//床の移動幅
	GLfloat mx;
	GLfloat my;
	GLfloat mz;
	
	//動く床の移動周期（片道の時間）
	GLint T;

	//動く床反転までの時間(片道)
	GLint reverseT;

	//床の速度（移動幅 / 時間）
	GLfloat vx;
	GLfloat vy;
	GLfloat vz;	
	
	//床の動く方向（周期Tで反転）
	GLint floorDirection;
	
	//床が動くかどうか(はじっこで止まる)
	bool bMove;

	//floorTimeが周期T以上になったら動く方向が反転
	GLint floorTime;
	
public:
	int stopT;
	void OnDraw();
	void OnDraw(GLfloat *x,GLfloat *y,GLfloat *z, bool *b);
	void InitInstance(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d);
	void InitInstance(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d, GLfloat mx, GLfloat my, GLfloat mz, GLfloat T);
	void SetVertex();
	void Judge(GLfloat *bx, GLfloat *by, GLfloat *bz, bool *bGravity);
	CMoveFloor();
	//CMoveFloor(GLfloat w,GLfloat h, GLfloat d);
	virtual ~CMoveFloor();

private:
};

}

#endif // !defined(AFX_MOVEFLOOR_H__91F5DF09_A733_47F4_834D_BFD580470BF5__INCLUDED_)
