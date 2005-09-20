#include "Stereo.h"
#include "LabelingCamera.h"

StereoV::StereoV(){
	nomalVision = 0;
	rightVision = 1;
	leftVision = 2;
	//parallax = PARALLAX / 2.0f;//ytakeyama comment out

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
	//parallax = PARALLAX / 2.0f;  //視差量の代入 ytakeyama comment out

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

Affinef StereoV::ShiftCamera(Affinef afC, Vec2f direction, float unitperm){//ytakeyama changed
	//ytakeyama 視差の計算を手直し
	Vec3f vecX,vecZ;
	float tempx,tempz;
	//vecX = Vec3f( afC.ExX(), afC.EyX(), afC.EzX());
	//vecZ = Vec3f( afC.ExZ(), afC.EyZ(), afC.EzZ());
	float para;
	if(whichEye == leftVision){ para =  PARALLAX * unitperm / 10.0;
	}else{ para = -PARALLAX * unitperm / 10.0;}
	//cout << "para = " << para << endl;
	tempz = para * afC.EzX();
	tempx = para * afC.EzZ();
	afC.PosX() += - tempx * direction.Y() - tempz * direction.X();
	afC.PosZ() += tempz * direction.Y() - tempx * direction.X();
	//cout << "EzX = " << afC.EzX() << endl;
	//cout << "EzZ = " << afC.EzZ() << endl;
	//cout << "tempz = " << tempz << endl;
	//cout << "tempx = " << tempx << endl;
	//cout << "dz = " << direction.Y() << endl;
	//cout << "dx = " << direction.X() << endl;
	//cout << "---------------------------" << endl;

/*
	afC.PosY() = 17.0;
	//視線を1.70mに設定
*/
	//cout << "Shift-Dir : " << direction << endl;

	//afC.PosY() = afC.PosY() + 17.0; //視線を1.7mに上げる　
	//vecParallax = Vec3f(parallax * direction.Y(), 0.0, - parallax * direction.X());
	//vecParallax = Vec3f(parallax, 0, 0);//ytakeyama comment out
	//vecParallax = Vec3f(PARALLAX * direction.Y(), 0,  - PARALLAX * direction.X());//ytakeyama
	/*
	vecParallax = Vec3f(PARALLAX * direction.Y(), 0,  - PARALLAX * direction.X());//ytakeyama

	//cout << "Parallax : " << vecParallax << endl;
	//cout << "Which ? :"<< whichEye << endl;
	if(whichEye == rightVision){
		afC.Pos() -= vecParallax;
		//cout << "Right" << endl;
	}else if(whichEye == leftVision){
		afC.Pos() += vecParallax;
		//cout << "Left" << endl;
	}
	//afC.Pos() += Vec3f(20.0, 0.0, 0.0);//ytakeyama test
	*/
	return afC;
}

void StereoV::InitParam(){

	pastStereoLC_X = 0; 
//	pastStereoLC_Y = 0; 
	pastStereoLC_Z = 0;
	
}

Vec3f StereoV::StereoLC(Vec3f *labelPos, Affinef afbody, int *flag){//ytakeyama changed メガネの位置を出すだけ

	stereoLC_X = -(labelPos[0].X() + labelPos[1].X())/2;
    stereoLC_Y = -(labelPos[0].Z() + labelPos[1].Z())/2;//ytakeyama respawn
	stereoLC_Z =  (labelPos[0].Y() + labelPos[1].Y())/2;
//　2点の中心座標をラベリングカメラ座標系から仮想世界座標系に代入

/*
	std::cout << "labelPos[0].X() = " << labelPos[0].X() << "   labelPos[1].X() = " << labelPos[1].X() << std::endl;
	std::cout << "labelPos[0].Y() = " << labelPos[0].Y() << "   labelPos[1].Y() = " << labelPos[1].Y() << std::endl;
*/

	gapStereoLC_X = stereoLC_X - pastStereoLC_X;
	gapStereoLC_Y = stereoLC_Y - pastStereoLC_Y;//ytakeyama respawn
//	gapStereoLC_Y  = 0.0;             //高さを考慮しないとき
	gapStereoLC_Z  = stereoLC_Z - pastStereoLC_Z;

	pastStereoLC_X = stereoLC_X;
	pastStereoLC_Y = stereoLC_Y;//ytakeyama respawn
	pastStereoLC_Z = stereoLC_Z;

	vecSLC = Vec3f( gapStereoLC_X, gapStereoLC_Y, gapStereoLC_Z);

	if (vecSLC.norm() <= 0.4) { vecSLC = Vec3f(0.0f, 0.0f, 0.0f); }
	//  視点がゆれるのを防ぐ

	//percentage = 0.1f;//ytakeyama
	percentage = 0.2f;

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

	//ytakeyama
	Vec3f tempVec3, zerovec;
	zerovec = Vec3f(0.0, -140.0, 0.0);
	tempVec3 = Vec3f(stereoLC_X , stereoLC_Y , stereoLC_Z);
	//cout << "x = " << stereoLC_X << endl << "z = " << stereoLC_Z << endl;
	//cout << "stereoLC_X = " << stereoLC_X << endl;
	//cout << "stereoLC_Y = " << stereoLC_Y << endl;
	//cout << "stereoLC_Z = " << stereoLC_Z << endl;
	*flag = 0;
	if(abs(stereoLC_X) < 300.0 && abs(stereoLC_Z) < 300.0 && -290 < stereoLC_Y && stereoLC_Y < -10.0){
		if (vecSLC.norm() <= 0.4) { *flag = 1;}
		return tempVec3;
	}else{
		//cout << "無駄！" << endl;
		return zerovec;
	}
	//return afbody;
	//end
}
