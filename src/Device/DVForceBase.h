#ifndef VR_DVFORCEBASE_H
#define VR_DVFORCEBASE_H

#include "DVVirtualDevice.h"
#include <base/Affine.h>

namespace Spr {
//----------------------------------------------------------------------------
//	DVForceBase

///	D/A��1�`�����l����\���D
class SPR_DLL DVForceBase:public DVVirtualDevice{
public:
	///	���R�x�̎擾
	virtual int GetDOF() const = 0;
	///	�͂̎擾
	virtual float GetForce(int ch) const = 0;
	///	�͂̎擾
	virtual void GetForce3(Vec3f& f) const{
		f[0] = GetForce(0);
		f[1] = GetForce(1);
		f[2] = GetForce(2);
	}
	///	�͂̎擾
	virtual void GetForce6(Vec3f& f, Vec3f& t) const{
		f.X() = GetForce(0);
		f.Y() = GetForce(1);
		f.Z() = GetForce(2);
		t.X() = GetForce(3);
		t.Y() = GetForce(4);
		t.Z() = GetForce(5);
	}
	///	�f�o�C�X�̖��O
	virtual const char* Name() const = 0;
	///	�f�o�C�X�̎��
	static const char* TypeS() { return "Force"; }
	///	�f�o�C�X�̎��
	virtual const char* Type() const{ return TypeS(); }
};

}	//	namespace Spr

#endif