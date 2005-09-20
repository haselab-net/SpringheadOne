// TGeoNodePointer.cpp: TGeoNodePointer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodePointer.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TGeoNodePointer::TGeoNodePointer()
{
	TGeoNode();
	isDraw = false;
	isAttack = false;
#ifdef ALGORITHM1
	g = -0.05;
#endif
#ifdef ALGORITHM2
	g = -0.06;
	//g = -0.01;
#endif
	dy = 0.0;
	dz = -1.0;
	radius = 4.0;
}

TGeoNodePointer::~TGeoNodePointer()
{
}

void TGeoNodePointer::Draw()
{
	if(!isDraw) return;


	// 位置を進める。VASHはDrawで判定しているのでこれもDrawで。
#ifdef ALGORITHM1
	dy += g;
#endif //ALGORITHM1

#ifdef ALGORITHM2
	count++;
	if( count < upcount || count > downcount ){
		dy += g;
	}
#endif

	posture = posture * Affinef(0,dy,dz);
//	Affinef afPos = afStart;// * posture;

	// 地面に落ちたら消える
	if( posture.PosY() < 0 ){
		Hide();
		return;
	}

/*
	// 一定距離進んだら消える
	if( posture.PosZ() < dz*50.0 ){
		Hide();
		return;
	}
*/
//	const char *pObjectNameStr = this->Name().c_str();
//	if( pObjectNameStr[5] == 's'){
//	}	

	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
//	glLoadIdentity();
	glMultMatrixf( posture );//* afPos  );	// 位置・姿勢を変更する。
//	glLoadMatrixf( afPos  );	// 位置・姿勢を変更する。
	glCallList(displayList);
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}

void TGeoNodePointer::Step()
{
}

bool TGeoNodePointer::Attack( Affinef &_afStart )
{
	if( isAttack ) return false;
	isDraw = true;
	isAttack = true;
	posture = _afStart;//_afCam * Affinef(0,-2,-1.5); // 初期座標をカメラ位置から少し下に
	dy = 0.35; // 初期速度
//	posture = Affinef(0,0,0);

	return true;
}

bool TGeoNodePointer::Attack( Affinef &afCam,Affinef &afLaser )
{
	if( isAttack ) return false;

	isDraw = true;
	isAttack = true;

	// 初期位置の設定
#ifdef ALGORITHM1
	afStart = afCam * Affinef(0,-2,-1.0); //ちょっとずらす;
	Affinef afPoint = afCam * afLaser * Affinef( 0,-2,-1.0);
	afStart.LookAtGL(afPoint.Pos());
	dy = 0.45;
//	posture = Affinef(0,0,0);
#endif //ALGORITHM1

#ifdef ALGORITHM2
	posture.Pos() = afCam.Pos(); //Vec3f( afCam.PosX(),afCam.PosY(),afCam.PosZ() );
	Affinef afPoint = afCam * afLaser;
	posture.LookAtGL( afPoint.Pos() );
	dy = sqrt( -2.0 * (afLaser.PosY()+2.0) * g );
	count = 0;
	upcount = - dy / g;
	downcount = 9;
	posture = posture * Affinef(0,-afLaser.PosY()-2.0,0);
#endif //ALGORITHM2

cout << "posture:" << posture.Pos() << endl;

	return true;
}

Affinef TGeoNodePointer::GetPosition()
{
	return posture;
}

void TGeoNodePointer::Hide()
{
	isDraw = false;
	isAttack = false;
}

float TGeoNodePointer::GetRadius()
{
	return radius;
}

bool TGeoNodePointer::IsDraw()
{
	return isDraw;
}
