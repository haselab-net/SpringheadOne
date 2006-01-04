#ifndef VR_DVORIENTATION_BASE_H
#define VR_DVORIENTATION_BASE_H

#include "DVVirtualDevice.h"
#include <base/Affine.h>

namespace Spr {
//----------------------------------------------------------------------------
//	DVOrientationBase

///	������Ԃ��f�o�C�X
class SPR_DLL DVOrientationBase:public DVVirtualDevice{
public:
	///	�����̎擾(��]�s��ł���킷)
	virtual Vec3f GetEuler() = 0;
	///	�����̎擾(��]�s��ł���킷)
	virtual void GetMatrix(Matrix3f& ori) = 0;
	///	�f�o�C�X�̖��O
	virtual const char* Name() const = 0;
	///	�f�o�C�X�̎��
	static const char* TypeS() { return "Orientation"; }
	///	�f�o�C�X�̎��
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif