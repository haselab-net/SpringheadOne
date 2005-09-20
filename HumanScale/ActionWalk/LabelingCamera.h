////���x�����O�J�����p<-���virtualDevice�ɂ��������ǂ��̂ł�

#include "WinBasis/WBWin.h"
#include "CySLC.h"
#include "SLCDevelop.h"
#include <Base/Affine.h>

using namespace LVRM;
using namespace std;

/*
typedef struct
{
	double	dX;			//���̂�X���W
	double	dY;			//���̂�Y���W
	double	dZ;			//���̂�Z���W
	int numLabel;		//���x�����ʔԍ�(0�ȏ�)
	int nStatus;			//�X�e�[�^�X 0:���� 1�ȏ�:�L��
} LabelData;
*/

class SLCamera{
protected:
	HANDLE lCamera;
	int labelNum;           //���x�����O�J�����Ŏ擾���郉�x���̐�
	enum {labelNumMax = 64};//�ꉞ64�ɂ��Ă��邪���Ȃ����Ă��\��Ȃ��̂ł́D

	int lastLabelNum;
	Vec2f eyeDirection,eyeDir,leftEye,rightEye,direction;
	Vec2f trackingEye[2],dLeftEye[2],dRightEye[2];
	float alpha; //eyeDirection�̕ω��ɑ΂���t�B���^�̌���
	Vec2f EyeDirection(); //�����������v�Z
	SLCOBJECT labelPos[labelNumMax]; //���x���̏����i�[����


public:
	SLCamera();
	bool InitSLC();
	void CloseSLCamera();
	bool Step();
	int GetLabelPos(Vec2f* labelPosition);//�����ɑ΂��ă��x���̍��W���i�[����܂��D�Ԃ�l�̓T�C�Y(���x���̐�)�ł��D
	int GetLabelPos(Vec3f* labelPosition);
	Vec2f GetEyeDirection(){ return eyeDirection; };//����������Ԃ��܂��D
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
			
	};//����������Ԃ��܂��DY���𒆐S�Ɋp�x������Ă܂��D
};

