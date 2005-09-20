// TGeoNodeVashPoint.cpp: TGeoNodeVashPoint クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodeVashPoint.h"
#include "GLView.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TGeoNodeVashPoint::TGeoNodeVashPoint()
{
	isDraw = false;
}

TGeoNodeVashPoint::~TGeoNodeVashPoint()
{

}

void TGeoNodeVashPoint::Draw()
{
	if(!isDraw) return;

	static int count = 0;
	if(count > 50){
		isDraw = false;
		count = 0;
	}
	count++;

	//
	Affinef af(Rad(90),'x');
	af = 0.004*af;
	af.Pos() = Vec3f(0,0,-0.1);
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( this->p_TGLView->afBody * af);	// 位置・姿勢を変更する。
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )

	//
}
