#ifndef VR_DVCOUNTERBASE_H
#define VR_DVCOUNTERBASE_H

#include "DVVirtualDevice.h"
namespace Spr {

//----------------------------------------------------------------------------
//	DVCountBase

///	�J�E���^��1�`�����l����\���D
class SPR_DLL DVCounterBase:public DVVirtualDevice{
public:
	DVCounterBase();
	///	�J�E���^�l�̐ݒ�
	virtual void Count(long count)=0;
	///	�J�E���^�l�̓ǂݏo��
	virtual long Count()=0;
	///	�f�o�C�X�̖��O
	virtual const char* Name() const=0;
	///	�f�o�C�X�̎��
	static const char* TypeS() { return "Counter"; }
	///	�f�o�C�X�̎��
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif
