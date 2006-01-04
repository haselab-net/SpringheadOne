#ifndef DEVICE_TOKIN3D_H
#define DEVICE_TOKIN3D_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DRRealDevice.h"
#include "DVOrientationBase.h"
#include "DVPioBase.h"
#include <base/Affine.h>

namespace Spr {
class SPR_DLL DRTokin3D: public DRRealDevice{
public:
	///	���z�f�o�C�X
	class DVOrientation:public DVOrientationBase{
	protected:
		DRTokin3D* realDevice;
	public:
		DVOrientation(DRTokin3D* r):realDevice(r){}
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual const char* Name() const{ return realDevice->Name(); }
		virtual void GetOrientationMatrix(Matrix3f& ori){
			realDevice->GetMatrix(ori);
		}
		virtual void GetMatrix(Matrix3f& ori){
			realDevice->GetMatrix(ori);
		}
		virtual Vec3f GetEuler(){
			return realDevice->GetEuler();
		}
	};
	class DVInputPort:public DVInputPortBase{
	protected:
		DRTokin3D* realDevice;
		int ch;
		char name[256];
	public:
		DVInputPort(DRTokin3D* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual const char* Name() const{ return name; }
		virtual int Get(){
			return realDevice->GetPort(ch);
		}
	};
protected:
	void* intf;
	char name[1024];
public:
	///
	DRTokin3D();
	///
	virtual ~DRTokin3D();
	///	������
	virtual bool Init();
	///	�o�^
	virtual void Register(DVVirtualDevicePool& vpool);
	///	���O
	virtual const char* Name() const { return name; }
	///	�����s��
	virtual void GetMatrix(Matrix3f& ori);
	///	�I�C���[�p�̎擾
	virtual Vec3f GetEuler();
	///	�|�[�g
	virtual int GetPort(int ch);
};

}	//	namespace Spr

#endif
