#ifndef VR_DVPIOBASE_H
#define VR_DVPIOBASE_H

#include "DVVirtualDevice.h"

namespace LVRM {;
//----------------------------------------------------------------------------
//	DVPioBase
///	���o�̓|�[�g�̂��߂̒萔�̒�`�Ȃǂ������s���D
class DVPortEnum{
	enum TLevel {LO, HI};
	enum TDir {IN, OUT};
};
///	���̓|�[�g��1�[�q������킷�D
class DVInputPortBase:public DVVirtualDevice, public DVPortEnum{
public:
	///	�f�o�C�X�̖��O
	virtual const char* Name() const =0;
	///	���̓|�[�g�̓d�����x��(Hi:1, Lo:0) ��Ԃ��D
	virtual int Get()=0;

	///	�f�o�C�X�̎��
	static const char* TypeS() { return "input port"; }
	///	�f�o�C�X�̎��
	virtual const char* Type() const{ return TypeS(); }
};

///	�o�̓|�[�g��1�[�q������킷�D
class DVOutputPortBase:public DVVirtualDevice, public DVPortEnum{
public:
	///	�f�o�C�X�̖��O
	virtual const char* Name() const =0;
	///	���̓|�[�g�̓d�����x��(Hi:1, Lo:0) ��Ԃ��D
	virtual void Set(int l)=0;

	///	�f�o�C�X�̎��
	static const char* TypeS() { return "output port"; }
	///	�f�o�C�X�̎��
	virtual const char* Type() const{ return TypeS(); }
};


///	PIO��1�[�q��\���D
class DVPioBase:public DVVirtualDevice, public DVPortEnum{
public:
	enum TLevel {LO, HI};
	///	�|�[�g�̃��W�b�N���x���̓��́BHi:true Lo:false
	virtual int Get()=0;
	///	�|�[�g�̃��W�b�N���x���̏o�́BHi:true Lo:false
	virtual void Set(int l)=0;
	///	�f�o�C�X�̖��O
	virtual const char* Name() const =0;
	///	�f�o�C�X�̎��
	static const char* TypeS() { return "PIO"; }
	///	�f�o�C�X�̎��
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace LVRM
#endif
