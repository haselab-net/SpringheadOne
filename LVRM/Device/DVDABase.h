#ifndef VR_DVDABASE_H
#define VR_DVDABASE_H

#include "DVVirtualDevice.h"

namespace LVRM {

//----------------------------------------------------------------------------
//	DVDaBase

///	D/A��1�`�����l����\���D
class DVDaBase:public DVVirtualDevice{
public:
	///	�o�͓d���̐ݒ�
	virtual void Voltage(float volt)=0;
	///	�f�o�C�X�̖��O
	virtual const char* Name() const =0;
	///	�f�o�C�X�̎��
	static const char* TypeS() { return "D/A"; }
	///	�f�o�C�X�̎��
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace LVRM

#endif