#include "HIS.h"
#pragma hdrstop
#include "HISpidarCanoe.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarCanoe, HISpidarG6);

HISpidarCanoe::HISpidarCanoe() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/

bool HISpidarCanoe::Init(DVDeviceManager& dev){
	//	糸のグリップへの取り付け位置
	const float GX = 1.27f/2.0f;		//	x方向の辺の長さ/2
	const float GY = 0.0f;				//	y方向の辺の長さ/2
	const float GZ = 0.40f/2.0f;		//	z方向の辺の長さ/2

#if 0	//	ミーティングルーム
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	const float PX = 2.76f/2;		//	x方向の辺の長さ/2
	const float PY = 2.50f/2;		//	y方向の辺の長さ/2
	const float PZ = 2.085f/2;		//	z方向の辺の長さ/2
	Vec3f motorPos[8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{Vec3f(-PX, PY, PZ), Vec3f( -GX, GY,  GZ)},//左上前
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, GY, -GZ)},//右上奥
		{Vec3f(-PX,-PY,-PZ), Vec3f( -GX, GY, -GZ)},//左下奥
		{Vec3f( PX,-PY, PZ), Vec3f(  GX, GY,  GZ)},//右下前
		{Vec3f( PX, PY, PZ), Vec3f(  GX, GY,  GZ)},//右上前
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, GY, -GZ)},//左上奥
		{Vec3f( PX,-PY,-PZ), Vec3f(  GX, GY, -GZ)},//右下奥
		{Vec3f(-PX,-PY, PZ), Vec3f( -GX, GY,  GZ)} //左下前
	};
#else	//	クロス
	const float PX = 0.25f/2;		//	x方向の辺の長さ/2
	const float PY = 2.35f/2;		//	y方向の辺の長さ/2
	const float PZ = 2.10f/2;		//	z方向の辺の長さ/2
	Vec3f motorPos[8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{Vec3f(-PX,-PY, PZ), Vec3f( -GX, GY,  GZ)},
		{Vec3f( PX,-PY, PZ), Vec3f(  GX, GY,  GZ)},
		{Vec3f( PX,-PY,-PZ), Vec3f(  GX, GY, -GZ)},
		{Vec3f(-PX,-PY,-PZ), Vec3f( -GX, GY, -GZ)},
		{Vec3f(-PX, PY, PZ), Vec3f( -GX, GY,  GZ)},
		{Vec3f( PX, PY, PZ), Vec3f(  GX, GY,  GZ)},
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, GY, -GZ)},
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, GY, -GZ)},
	};
#endif
	/*
	Vec3f motorPos[8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{Vec3f(-PX,-PY, PZ), Vec3f(0.0f,  -GY, 0.0f)},
		{Vec3f(-PX,-PY,-PZ), Vec3f( -GX, 0.0f, 0.0f)},
		{Vec3f( PX,-PY,-PZ), Vec3f(  GX, 0.0f, 0.0f)},
		{Vec3f( PX,-PY, PZ), Vec3f(0.0f,  -GY, 0.0f)},
		{Vec3f(-PX, PY, PZ), Vec3f(0.0f,   GY, 0.0f)},
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, 0.0f, 0.0f)},
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, 0.0f, 0.0f)},
		{Vec3f( PX, PY, PZ), Vec3f(0.0f,   GY, 0.0f)}
	};
	*/
/*
	//キャリブレーション位置を中心からずらす
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/
	double lpp = 3.0824008138351983723296032553408e-5 * 500 /1024;
	if( HISpidarG6::Init(dev, 8, motorPos, 0.3f, (float)lpp, 1.0f, 20.0f) == false ){
		return false;
	}
	motor[1].lengthPerPulse *= -1;
	motor[3].lengthPerPulse *= -1;
	motor[4].lengthPerPulse *= -1;
	motor[6].lengthPerPulse *= -1;
	/*
	motor[4].lengthPerPulse *= -1;
	motor[5].lengthPerPulse *= -1;
	motor[6].lengthPerPulse *= -1;
	motor[7].lengthPerPulse *= -1;
	*/

	Calib();
	return true;
}

}//	namespace Spr
