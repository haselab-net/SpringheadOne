////ラベリングカメラ用<-後でvirtualDeviceにした方が良いのでは

#ifndef __LABELING_CAMERA_H__
#define __LABELING_CAMERA_H__

#include <Base/Affine.h>
#include <WinBasis/WBWin.h>
#include "CySLC.h"
#include "SLCDevelop.h"
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace LVRM;

#define PI1 3.14159265358979
/*
typedef struct
{
	double	dX;			//物体のX座標
	double	dY;			//物体のY座標
	double	dZ;			//物体のZ座標
	int numLabel;		//ラベル識別番号(0以上)
	int nStatus;			//ステータス 0:無効 1以上:有効
} LabelData;
*/

class SLCamera{
protected:
	HANDLE lCamera;
	int labelNum;           //ラベリングカメラで取得するラベルの数
	enum {labelNumMax = 64};//一応64にしてあるが少なくしても構わないのでは．
	int lastLabelNum;
	Vec2f eyeDirection,eyeDir,direction;
	Vec2f leftEye2D, rightEye2D;
	Vec3f leftEye, rightEye, topEye;
	Vec3f centerPoint, rightEyeVec, thirdEyeVec;
	enum {LABEL_NUM = 3};
	Vec3f trackingEye[LABEL_NUM],dtopEye[LABEL_NUM],dLeftEye[LABEL_NUM],dRightEye[LABEL_NUM];
	float alpha, tmp; //eyeDirectionの変化に対するフィルタの厚さ
	Vec2f EyeDirection(); //視線方向を計算
	SLCOBJECT labelPos[labelNumMax]; //ラベルの情報を格納する

	Vec3f eyeDirection3D;
	int AngleSide;			//X-Z平面の角度
	int AngleVert;			//Y-Z平面の角度
	int PreLabelNum;		//ラベリングの数
	Vec3f eyeDirection2;//視線方向
	int Angle[2][2];	//[0:AngleSide,1:AngleVert][0:一つ前、1:現在]
//	int *PreEyeAngle;

	Affined IdealPos;
	Affined CallibAffin;
	Vec4d NowEyePos[3];
	bool Calib;


public:
	//Vec3f eyeDirection3D;
	SLCamera();
	~SLCamera();
	bool InitSLC(); //new
	//bool InitSLC(int* PreMainEyeAngle);
	void CloseSLCamera();
	bool Step();			//位置取得、視線方向、角度計算
	
	int GetLabelPos(Vec2f* labelPosition);//引数に対してラベルの座標が格納されます．返り値はサイズ(ラベルの数)です．
	int GetLabelPos(Vec3f* labelPosition);
	Vec2f GetEyeDirection(){ return eyeDirection; };//視線方向を返します．
	float GetEyeAngle();

	Vec3f GetEyeDirection2();	//視線方向を計算osada版
	void ShowData();		//各角度を表示
	int AdjustAngleSide();	//水平方向の角度を調整
	int AdjustAngleVert();	//垂直方向の角度を調整
	float GetEyeAngleSide();//水平方向の角度を取得
	float GetEyeAngleVert();//水平方向の角度を取得
	//10/21
	void CallibSLC();		//カメラのキャリブレーション

};

#endif