#include <Base/Affine.h>
#include "WinBasis/WBWin.h"
#include "CySLC.h"

using namespace LVRM;

#define PARALLAX 0.65f

typedef struct
{
	float past[3];
	float gap[3];
	float now[3];
} Stereo;

class StereoV{
public:
	StereoV();
	StereoV(int pcNumber);
	Affinef StereoLC(Vec3f *labelPos, Affinef afbody);
	Affinef ShiftCamera(Affinef afC, Vec2f direction);
	void InitParam();
	void SetParallax(float setPara) {parallax = setPara;}

protected:
	int nomalVision,rightVision,leftVision;

	int WhichVision(int pcNum);
	int whichEye;
	int pcNum; //マシン番号　＃後で使うかもしれないから

	float parallax;     //視差の値
	Vec3f vecParallax;  //視差を反映させるためのベクトル
	float percentage;   //マップデータの値とラベリングカメラの値を対応させる

	float stereoLC_X, stereoLC_Y, stereoLC_Z;  //ラベリングカメラの値
	float pastStereoLC_X, pastStereoLC_Y, pastStereoLC_Z;  //１つ前のラベリングカメラの値
	float gapStereoLC_X, gapStereoLC_Y, gapStereoLC_Z;

	Vec3f vecSLC;  //afbodyを更新する値
	Vec3f vecEX, vecEY, vecEZ;  //アバターの座標系

//	float posOfPivot;        //回転の中心
//	double lookInsideAngle;  //回転角度

};