#include "Stereo.h"
#include "LabelingCamera.h"

StereoV::StereoV(){
	nomalVision = 0;
	rightVision = 1;
	leftVision = 2;
	parallax = PARALLAX / 2.0f;

	whichEye = nomalVision;
	pcNum = -1;

	stereoLC_X = 0;
	stereoLC_Y = 0;
	stereoLC_Z = 0;
	pastStereoLC_X = 0; 
	pastStereoLC_Y = 0; 
	pastStereoLC_Z = 0;
	gapStereoLC_X = 0.0;
	gapStereoLC_Y = 0.0;
	gapStereoLC_Z = 0.0;

	vecEX = Vec3f(0.0f, 0.0f, 0.0f);
	vecEY = Vec3f(0.0f, 0.0f, 0.0f);
	vecEZ = Vec3f(0.0f, 0.0f, 0.0f);

	vecSLC = Vec3f(0.0f, 0.0f, 0.0f);
}

StereoV::StereoV(int pcNumber){
	nomalVision = 0;
	rightVision = 1;
	leftVision = 2;
	parallax = PARALLAX / 2.0f;  //視差量の代入

	pastStereoLC_X = 0; 
	pastStereoLC_Y = 0; 
	pastStereoLC_Z = 0;
	whichEye = WhichVision(pcNumber);
	pcNum = pcNumber;

	stereoLC_X = 0;
	stereoLC_Y = 0;
	stereoLC_Z = 0;
	gapStereoLC_X = 0.0;
	gapStereoLC_Y = 0.0;
	gapStereoLC_Z = 0.0;

	vecEX = Vec3f(0.0f, 0.0f, 0.0f);
	vecEY = Vec3f(0.0f, 0.0f, 0.0f);
	vecEZ = Vec3f(0.0f, 0.0f, 0.0f);

	vecSLC = Vec3f(0.0f, 0.0f, 0.0f);
}

int StereoV::WhichVision(int pcNum){
	switch(pcNum){
	case 0:   case 1:   case 2:   case 3:
	case 15:  case 14:  case 17:  case 16:
	case 20:  case 30:  case 21:  case 31:
		return rightVision;
	default:
		return leftVision;
	}
}

Affinef StereoV::ShiftCamera(Affinef afC, Vec2f direction){
/*
	afC.PosY() = 17.0;
	//視線を1.70mに設定
*/
	//cout << "Shift-Dir : " << direction << endl;

	afC.PosY() = afC.PosY() + 0.0; //視線を1.7mに上げる　この方法でないと高さが変えられない.ただし足が浮いている状態になる.
	vecParallax = Vec3f(parallax * direction.Y(), 0.0, - parallax * direction.X());
	//vecParallax = Vec3f(parallax * direction.X(), 0.0, - parallax * direction.Y());
	
	//cout << "Parallax : " << vecParallax << endl;
	//cout << "Which ? :"<< whichEye << endl;
	if(whichEye == rightVision)
		afC.Pos() -= vecParallax;
	else if(whichEye == leftVision)
		afC.Pos() += vecParallax;
	return afC;
}

void StereoV::InitParam(){

	pastStereoLC_X = 0; 
//	pastStereoLC_Y = 0; 
	pastStereoLC_Z = 0;
}

Affinef StereoV::StereoLC(Vec3f *labelPos, Affinef afbody){

	stereoLC_X = -(labelPos[0].X() + labelPos[1].X())/2;
//    stereoLC_Y = -(labelPos[0].Z() + labelPos[1].Z())/2;
	stereoLC_Z =  (labelPos[0].Y() + labelPos[1].Y())/2;
//　2点の中心座標をラベリングカメラ座標系から仮想世界座標系に代入

/*
	std::cout << "labelPos[0].X() = " << labelPos[0].X() << "   labelPos[1].X() = " << labelPos[1].X() << std::endl;
	std::cout << "labelPos[0].Y() = " << labelPos[0].Y() << "   labelPos[1].Y() = " << labelPos[1].Y() << std::endl;
*/

	gapStereoLC_X = stereoLC_X - pastStereoLC_X;
//	gapStereoLC_Y = stereoLC_Y - pastStereoLC_Y;
	gapStereoLC_Y  = 0.0;             //高さを考慮しないとき
	gapStereoLC_Z  = stereoLC_Z - pastStereoLC_Z;

	pastStereoLC_X = stereoLC_X;
//	pastStereoLC_Y = stereoLC_Y;
	pastStereoLC_Z = stereoLC_Z;

	vecSLC = Vec3f( gapStereoLC_X, gapStereoLC_Y, gapStereoLC_Z);

	if (vecSLC.norm() <= 0.4) { vecSLC = Vec3f(0.0f, 0.0f, 0.0f); }
	//  視点がゆれるのを防ぐ

	percentage = 0.1f;

	vecEX = Vec3f( afbody.ExX(), afbody.EyX(), afbody.EzX());
//	vecEY = Vec3f( afbody.ExY(), afbody.EyY(), afbody.EzY());
	vecEZ = Vec3f( afbody.ExZ(), afbody.EyZ(), afbody.EzZ());
	afbody.PosX() = percentage * vecEX * vecSLC + afbody.PosX();
//	afbody.PosY() = percentage * vecEY * vecSLC + afbody.PosY();
	afbody.PosZ() = percentage * vecEZ * vecSLC + afbody.PosZ();
//	cout << "afbody.ExX = " << afbody.ExX() << "afbody.ExZ = " << afbody.ExZ() << endl;

/////
//  視点方向も変えるとき
//	float posOfPivot = -500;
//	lookInsideAngle = atan(stereoLC_X / (stereoLC_Z - posOfPivot));
//	afbody = afbody * Affinef(lookInsideAngle, 'Y');
/////
	return afbody;
}
