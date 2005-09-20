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