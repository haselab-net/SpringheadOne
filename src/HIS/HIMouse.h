#ifndef HIMOUSE_H
#define HIMOUSE_H

#include "HIHapticDevice.h"
#include <Device/DVDeviceManager.h>

namespace Spr {;

class SPR_DLL HIMouse: public HIForceDevice6D{
public:
	/// ������Ă���{�^���̏�Ԃ������t���O
	enum ButtonState{
		NONE,
		// #define MK_LBUTTON          0x0001
		LBUTTON = MK_LBUTTON,
		// #define MK_RBUTTON          0x0002
		RBUTTON = MK_RBUTTON,
		// #define MK_SHIFT            0x0004
		SHIFT   = MK_SHIFT,
		// #define MK_CONTROL          0x0008
		CONTROL = MK_CONTROL,
		// #define MK_MBUTTON          0x0010
		MBUTTON = MK_MBUTTON,
	} btnState;	

	/// 
	HIMouse(){ bGood = false; }
	virtual ~HIMouse(){}
	///	�f�o�C�X�̃^�C�v
	HI_DEVICE_TYPE_DEF("Mouse")

	bool IsGood(){return bGood;}

	/// �}�E�X�̏�����
	bool Init();

	///	������Ă���{�^���ɉ�����pos,ori���X�V����BShift��������Ă���Ƃ���axis���X�V����B
	void Update(float dt);

	///	�܂Ƃ߂āC���b�Z�[�W����������ꍇ
	bool PreviewMessage(void* m);
	///@name �ʂ̃��b�Z�[�W�n���h���ŌĂяo���ꍇ
	//@{
	///	�L�[����
	bool OnKeyDown(unsigned nChar);
	/// �h���b�O���̏���
	void OnButtonMove(UINT state, int x, int y);
	/// �{�^�����������Ƃ��̏���
	void OnButtonDown(int x, int y);
	/// �{�^���𗣂����Ƃ��̏���
	void OnButtonUp();
	///	�_�u���N���b�N���̏���
	void OnDblClick(unsigned fwKeys, int x, int y);
	/// ���_�u���N���b�N���̏���(�|�C���^�̎p���𐅕��ɏ�����)
	void OnLButtonDClick();
	/// �E�_�u���N���b�N���̏���(�|�C���^�̈ʒu�����_�ɏ�����)
	void OnRButtonDClick();
	/// Shift+���_�u���N���b�N���̏���(����W���̈ʒu�E�p����������)
	void OnShiftLButtonDClick();
	/// �z�C�[�����܂킵���Ƃ��̏���(�|�C���^or����W����Z�������ɐ����ړ�)
	void OnWheel(UINT state, short dz);
	//@}

	///	�ʒu�̎擾
	Vec3f GetPos(){ return pos; }
	///	�͂̎擾
	Vec3f GetForce(){ return force; }
	///	�g���N�̎擾
	Vec3f GetTorque(){ return torque; }
	///	�p�x�̎擾
	Quaternionf GetOri(){ return ori; }
	/// ����W���̎擾
	Affinef GetAxis(){ return axis; }

	/// �X�P�[���̐ݒ�
	void SetScale(float tp, float rp, float ta, float ra);

	/// ���ʂ̐ݒ�
	void SetMass(float m){ mass = m; }
	/// �ʒu�̐ݒ�
	void SetPos(Vec3f p){ pos = p; }
	/// �p���̐ݒ�
	void SetOri(Quaternionf o){ ori = o; }
	///	���i�́E��]�͂̐ݒ�
	void SetForce(const Vec3f& f, const Vec3f& t){ force = f; torque = t; }
	void SetForce(const Vec3f& f){ force = f; }
	///	�Œ�̗͂�ݒ肷��
	void SetMinForce(){ force = torque = Vec3f(); }	

	/// ����W���̐ݒ�
	void SetAxis(Affinef a){ axis = a; }
	/// �������p���W���̐ݒ�
	void SetAxisInit(Affinef ai){ axisInit = ai; }


	///	�L�����u���[�V����(���ɉ������Ȃ�)
	bool Calib(){ return true; }

protected:
	// �}�E�X���A�N�e�B�u���ǂ����̃t���O
	bool bGood;
	// �}�E�X�̍��W(old:�O�X�e�b�v�Cnew:���X�e�b�v)
	int oldX, oldY, newX, newY;
	// �}�E�X�̍��W�Ɖ��z���E�Ƃ̃X�P�[�������p�p�����[�^
	float scaleTransPointer, scaleRotPointer, scaleTransAxis, scaleRotAxis;

	// �|�C���^�Ɋւ���ϐ�
	// ����
	float mass;
	// �|�C���^�ɂ�������i��
	Vec3f force;
	// �|�C���^�ɂ������]��
	Vec3f torque;
	// �|�C���^�̈ʒu�BUpdate()�ōX�V�����
	Vec3f pos;
	// �|�C���^�̎p���BUpdate()�ōX�V�����
	Quaternionf ori;

	// ����W��(�h���b�O���쎞�͂��̍��W���ɂ��ǂÂ��ē��������)
	Affinef axis;
	// �������p���W��
	Affinef axisInit;
};

}	//	namespace Spr
#endif
