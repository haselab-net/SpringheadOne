#ifndef VR_DVADBASE_H
#define VR_DVADBASE_H

#include "DVVirtualDevice.h"

namespace LVRM {

//----------------------------------------------------------------------------
//	DVAdBase

///	A/D��1�`�����l����\���D
class DVAdBase:public DVVirtualDevice{
public:
	///	���͓d���̎擾
	virtual float Voltage()=0;
	///	�f�o�C�X�̖��O
	virtual const char* Name() const =0;
	///	�f�o�C�X�̎��
	static const char* TypeS() { return "A/D"; }
	///	�f�o�C�X�̎��
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace LVRM

#endif