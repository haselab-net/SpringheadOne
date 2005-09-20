#include <Base/Affine.h>
#include "WinBasis/WBWin.h"
#include "CySLC.h"
#include <math.h>//ytakeyama

using namespace LVRM;

//#define PARALLAX 0.325f
#define PARALLAX 0.0f

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
	Vec3f StereoLC(Vec3f *labelPos, Affinef afbody,int* flag);//ytakeyama
	Affinef ShiftCamera(Affinef afC, Vec2f direction, float unitperm);//ytakeyama changed
	void InitParam();
	void SetParallax(float setPara) {parallax = setPara;}
	//Vec3f vecSLC_copy;
	

protected:
	int nomalVision,rightVision,leftVision;

	int WhichVision(int pcNum);
	int whichEye;
	int pcNum; //�}�V���ԍ��@����Ŏg����������Ȃ�����

	float parallax;     //�����̒l
	Vec3f vecParallax;  //�����𔽉f�����邽�߂̃x�N�g��
	float percentage;   //�}�b�v�f�[�^�̒l�ƃ��x�����O�J�����̒l��Ή�������

	float stereoLC_X, stereoLC_Y, stereoLC_Z;  //���x�����O�J�����̒l
	float pastStereoLC_X, pastStereoLC_Y, pastStereoLC_Z;  //�P�O�̃��x�����O�J�����̒l
	float gapStereoLC_X, gapStereoLC_Y, gapStereoLC_Z;

	Vec3f vecSLC;  //afbody���X�V����l
	Vec3f vecEX, vecEY, vecEZ;  //�A�o�^�[�̍��W�n

//	float posOfPivot;        //��]�̒��S
//	double lookInsideAngle;  //��]�p�x

};