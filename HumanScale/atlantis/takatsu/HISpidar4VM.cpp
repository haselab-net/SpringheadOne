#include "HIS.h"
#pragma hdrstop
#include "HISpidar4VM.h"

namespace Spr {;
//----------------------------------------------------------------------------
HISpidar4VM::HISpidar4VM() : HISpidar4() {
}

bool HISpidar4VM::Init(DVDeviceManager& dev){

	// VM室Big-SM_PIDAR用　モータの取り付け座標（ターンテーブルの中心から床上110cmが原点）	
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.75f, 2.00f, 1.12f),	Vec3f( 1.65f, 2.17f,-1.34f), Vec3f(-1.65f,-0.9f,-1.44f), Vec3f( 1.65f,-0.99f, 0.50f)},
		{Vec3f( 1.58f, 2.00f, 1.12f),	Vec3f(-1.65f, 2.18f,-1.34f), Vec3f( 1.65f,-0.98f,-1.44f), Vec3f(-1.65f,-0.98f, 0.50f)}
	};*/
	// 2003.10 NEW SETTING
	//モータの取り付け位置(中心を原点とする)

	Vec3f motorPos[][4] = {
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};


	if( HISpidar4::Init(dev, motorPos[0], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f) == false ){
		return false;
	}
	if( HISpidar4::Init(dev, motorPos[1], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f) == false ){
		return false;
	}
	//Calib();
	return true;
}

}//	namespace Spr