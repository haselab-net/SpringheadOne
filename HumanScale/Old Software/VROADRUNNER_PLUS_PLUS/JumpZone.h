// JumpZone.h: CJumpZone クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JUMPZONE_H__494B14C0_5F5D_47B8_AD8A_40700F143322__INCLUDED_)
#define AFX_JUMPZONE_H__494B14C0_5F5D_47B8_AD8A_40700F143322__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <GL/glut.h>
#include <stdio.h>
#include <Base/VRBase.h>

using namespace LVRM;
namespace vroadrunner {

class CJumpZone  
{
public:
	//ジャンプゾーンの場所(始点の位置）
	GLfloat x;
	GLfloat y;
	GLfloat z;

	//ジャンプゾーンの大きさ
	GLfloat w;
	GLfloat d;
	GLfloat h;
	
	//飛ぶ距離＆高さ
	GLint jumpDist;
	
public:
	void InitInstance(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d, GLint jumpDist);
	void OnDraw(Affinef *afBody, Vec3f *dvel, float accel);
	CJumpZone();
	virtual ~CJumpZone();
	bool GetisJump(){ return isJump; };
	bool isJump;

};

}

#endif // !defined(AFX_JUMPZONE_H__494B14C0_5F5D_47B8_AD8A_40700F143322__INCLUDED_)


