#ifndef VR_DVDEVICEBASE_H
#define VR_DVDEVICEBASE_H

#include <Base/BaseDebug.h>
#include <Base/BaseUtility.h>
#include <iosfwd>

namespace Spr {

//----------------------------------------------------------------------------
//	DVVirtualDevice

class DRRealDevice;

///	���z�f�o�C�X�̊�{�N���X�DDA,�J�E���^�Ȃǂ�1�`�����l���ɑ�������.
class SPR_DLL DVVirtualDevice:public UTRefCount{
	bool used;
	virtual bool GetUsed(){return used;}
	virtual void SetUsed(bool b){used = b;}
	friend class DVVirtualDevicePool;
public:
	DVVirtualDevice(){ used = false; }
	virtual ~DVVirtualDevice(){}
	///	�f�o�C�X�̖��O
	virtual const char* Name() const=0;
	///	�f�o�C�X�̎��
	virtual const char* Type() const=0;
	///	���f�o�C�X�ւ̃|�C���^
	virtual DRRealDevice* RealDevice()=0;
	///	�_���v�o��
	virtual void Print(std::ostream& o) const;
	///
	virtual void Update(){}
};
inline std::ostream& operator << (std::ostream& o, const DVVirtualDevice& d){
	d.Print(o);
	return o;
}
}	//	namespace Spr

#endif
