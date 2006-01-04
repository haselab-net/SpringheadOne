#ifndef HISPIDAR4_H
#define HISPIDAR4_H

#include "HIHapticDevice.h"
#include "HISpidarMotor.h"
#include "Device/DVDeviceManager.h"
#include <vector>

namespace Spr {

/**	4�{����SPIDAR.
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

class SPR_DLL HISpidar4: public HIForceDevice3D{
public:

	typedef HIForceDevice3D superclass;
protected:
	Vec3f pos;
	///	�ʒu���v�Z���邽�߂̒萔
	Vec3f posSqrConst;
	///	�ʒu���v�Z���邽�߂̍s��
	Matrix3f matPos;
	Vec3f phi[4];
	float tension[4];	// computed tension
	
protected:
	///	���[�^
	HISpidarMotor motor[4];

	/**	�o�͉\�ȗ͂��v�Z���A���̏o�͂̓��_���v�Z����.
		@param disable	�g�p���Ȃ����̔ԍ�
		@param f		�o�͂�������. �o�͉\�ȗ͂ɕύX�����.
		@param v3Str	4�{�̎��̌���
		@return ���_(�͂̐��m���ƃ��[�^�o�͂̑傫���Ō��܂�B
				���̎���I�����邱�Ƃ̐�����)	*/
	float AvailableForce(int disable, Vec3f& f, Vec3f* v3Str);
	///	�ʒu���v�Z���邽�߂̍s��̏�����
	void InitMat();

public:
	///
	HISpidar4();
	///	�f�o�C�X�̃^�C�v
	HIOBJECTDEF(HISpidar4);
	///
	virtual ~HISpidar4();
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev);
	///	�f�o�C�X�̏�����(�g�p�O�ɌĂ�)
	virtual bool Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF);
	///	�L�����u���[�V�����̑O�ɍs������
	virtual bool BeforeCalib(){ SetMinForce(); return true; }
	///	�L�����u���[�V����
	virtual bool Calib();
	///	�Œᒣ�͂��o��
	void SetMinForce();
	///	�͂̏o��
	virtual void SetForce(const Vec3f& f, const Vec3f& t){ SetForce(f); }
	virtual void SetForce(const Vec3f& f){ SetForce(f, 0.1f); }
	virtual void SetForce(const Vec3f& f,float eff){ SetForce(f, eff, 0.0f); }
	virtual void SetForce(const Vec3f& f, float eff, float cont);
	///	�f�o�C�X�̏�Ԃ��X�V����.
	virtual void Update(float dt);
	virtual Vec3f GetForce();
	virtual Vec3f GetPos(){ return pos; }

	HISpidarMotor* Motor(){
		return motor;
	}

};


}	//	namespace Spr
#endif

