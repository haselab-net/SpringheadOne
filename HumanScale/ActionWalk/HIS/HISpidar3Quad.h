#ifndef HDSPIDAR3QUAD_H
#define HDSPIDAR3QUAD_H

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVDeviceManager.h"
#include <vector>

namespace LVRM {
using namespace LVRM;

/**	3�{����SPIDAR.
	���̒���������W�v�Z���s������A�o�͂���͊o����e���[�^��
	�o�͂��v�Z�����肷��B�o�͂̌v�Z��2���v��@��p���Ă��邽�߁C
	HIS/Fortran �� libF77.lib libI77.lib QuadPR.lib ���K�v�D
@verbatim
SPIDAR�̒萔�̐ݒ��
Init(dev, motPos, 0.5776, 2.924062107079e-5f, 0.5f, 20.0f);

//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
#define PX	0.265f	//	x�����̕ӂ̒���/2
#define PY	0.265f	//	y�����̕ӂ̒���/2
#define PZ	0.265f	//	z�����̕ӂ̒���/2
Vec3f motorPos[][4] =	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���){
	{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
	{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
};

//	�P�j���[�g���o�����߂�DA�̏o�͓d��[V/N]
//	maxon:		0.008[m] / 0.0438[Nm/A] / 0.5[A/V] =  0.365296803653
//	��t���@:	0.008[m] / 0.0277[Nm/A] / 0.5[A/V] =  0.5776
sp->VoltPerNewton(0.5776f);

//	�P�J�E���g�̒��� [m/plus]	�����l: 2.924062107079e-5f
sp->LenPerPlus(2.924062107079e-5f);

//	�ŏ����� [N]
sp->MinForce(0.5f);

//	�ő咣�� [N]
sp->MaxForce(20.0f);
@endverbatim
*/

class HISpidar3Quad: public HDForceDevice3D{
public:

	typedef HDForceDevice3D superclass;
protected:
	Vec3f pos;
	///	���[�^
	HISpidarMotor motor[3];
	///	�ʂ̂ǂ��瑤���g�p���邩
	bool bLeft;

public:
	///
	HISpidar3Quad();
	///
	virtual ~HISpidar3Quad();
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev);
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF, bool bLeft);
	///	�L�����u���[�V�����̑O�ɍs������
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	�L�����u���[�V����
	virtual bool Calib();
	///	�ȈՃL�����u���[�V����(���_�ł͂Ȃ��C���[�^3�̏d�S�ŃL�����u���[�V����)
	virtual bool EasyCalib();
	///	�Œᒣ�͂��o��
	void SetMinForce();
	///	�͂̏o��
	virtual void SetForce(const Vec3f& f){ SetForce(f, 0.1f); }
	virtual void SetForce(const Vec3f& f, float gamma);
	///	�f�o�C�X�̏�Ԃ��X�V����.
	virtual void Update();

	Vec3f CalcForce(Vec3f *f);
	
	HISpidarMotor* Motor(){
		return motor;
	}

};


}	//	namespace LVRM
#endif

