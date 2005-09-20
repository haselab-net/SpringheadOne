////ラベリングカメラ用<-後でvirtualDeviceにした方が良いのでは

#include "WinBasis/WBWin.h"
#include "CySLC.h"
#include "SLCDevelop.h"
#include <Base/Affine.h>

using namespace LVRM;
using namespace std;

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
	Vec2f eyeDirection,eyeDir,direction, leftEye2D, rightEye2D;
	Vec3f leftEye, rightEye, topEye;
	Vec3f centerPoint, rightEyeVec, thirdEyeVec;
	enum {LABEL_NUM = 3};
	Vec3f trackingEye[LABEL_NUM],dtopEye[LABEL_NUM],dLeftEye[LABEL_NUM],dRightEye[LABEL_NUM];
	float alpha, tmp; //eyeDirectionの変化に対するフィルタの厚さ
	Vec2f EyeDirection(); //視線方向を計算
	SLCOBJECT labelPos[labelNumMax]; //ラベルの情報を格納する


public:
	Vec3f eyeDirection3D;
	SLCamera();
	bool InitSLC();
	void CloseSLCamera();
	bool Step();
	int GetLabelPos(Vec2f* labelPosition);//引数に対してラベルの座標が格納されます．返り値はサイズ(ラベルの数)です．
	int GetLabelPos(Vec3f* labelPosition);
	Vec2f GetEyeDirection(){ return eyeDirection; };//視線方向を返します．
	float GetEyeAngle(){ 
		if(eyeDirection.Y() != 0.0f){
			return atan2(eyeDirection.X(),eyeDirection.Y());
		}
		else if(eyeDirection.X()>0){
			return M_PI / 2;
		}
		else{
			return -M_PI / 2;
		}
			
	};//視線方向を返します．Y軸を中心に角度を取ってます．
};

