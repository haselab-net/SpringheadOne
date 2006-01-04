#ifndef HIORIENTATION_H
#define HIORIENTATION_H

#include "HIHapticDevice.h"
#include <Device/DVDeviceManager.h>
#include <Device/DVOrientationBase.h>

namespace Spr {;

class SPR_DLL HIOrientation: public HIForceDevice6D{
public:
	///	�f�o�C�X�̃^�C�v
	HIOBJECTDEFABST(HIOrientation);

	DVOrientationBase* dev;
	/// 
	HIOrientation(){ bGood = false; }
	virtual ~HIOrientation(){}

	bool IsGood(){return bGood;}

	bool Init(DVDeviceManager& devMan);

	void Update(float dt);

	///	�ʒu�̎擾
	Vec3f GetPos(){ return pos; }
	///	�͂̎擾
	Vec3f GetForce(){ return force; }
	///	�g���N�̎擾
	Vec3f GetTorque(){ return torque; }
	///	�p�x�̎擾
	Quaternionf GetOri(){ return ori; }

	/// �ʒu�̐ݒ�
	void SetPos(Vec3f p){ pos = p; }
	/// �p���̐ݒ�
	void SetOri(Quaternionf o){ ori = o; }
	///	���i�́E��]�͂̐ݒ�
	void SetForce(const Vec3f& f, const Vec3f& t){ force = f; torque = t; }
	void SetForce(const Vec3f& f){ force = f; }
	///	�Œ�̗͂�ݒ肷��
	void SetMinForce(){ force = torque = Vec3f(); }	


	///	�L�����u���[�V����(���ɉ������Ȃ�)
	bool Calib(){ return true; }

protected:
	// �}�E�X���A�N�e�B�u���ǂ����̃t���O
	bool bGood;
	// �|�C���^�ɂ�������i��
	Vec3f force;
	// �|�C���^�ɂ������]��
	Vec3f torque;
	// �|�C���^�̈ʒu�BUpdate()�ōX�V�����
	Vec3f pos;
	// �|�C���^�̎p���BUpdate()�ōX�V�����
	Quaternionf ori;
};

}	//	namespace Spr
#endif
