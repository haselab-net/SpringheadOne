#ifndef VR_DVDEVICES_H
#define VR_DVDEVICES_H
#include <Device/DVVirtualDevice.h>
#include <vector>

namespace Spr {

//----------------------------------------------------------------------------
//	DVVirtualDevices

/**	���z�f�o�C�X�̃v�[��. ISA�J�[�h��PC�J�[�h�Ȃǂ̃f�o�C�X�́C
	�@�\(D/A,�J�E���^�Ȃ�)�ʂɕ������C���z�f�o�C�X�Ƃ��Ă���
	�v�[���ɓo�^�����D
	�o�^���ꂽ�f�o�C�X�́CSPIDAR�Ȃǂ̃V�X�e���ɗ��p�����D	*/

class SPR_DLL DVVirtualDevicePool: std::vector< UTRef<DVVirtualDevice> >{
public:
	///	Rent device.
#ifdef __BORLANDC__
	template <class CT> bool Rent(CT*& t);
#else
	template <class CT> bool Rent(CT*& t){
		CT* pt = (CT*)Rent(CT::TypeS());
		t = pt;
		return t != NULL;
	}
#endif
	///	Rent device.
	DVVirtualDevice* Rent(const char* type, const char* name=NULL);
	///	return device.
	bool Return(DVVirtualDevice* dev);
	///	Register virutal device.
	void Register(DVVirtualDevice* dev){ push_back(dev); }
	///	�_���v�o��
	void Print(std::ostream& o) const;
	///
	void Clear(){ clear(); }
};
#ifdef __BORLANDC__
template <class CT> bool DVVirtualDevicePool::Rent(CT*& t){
		CT* pt = (CT*)Rent(CT::TypeS());
		t = pt;
		return t != NULL;
}
#endif
inline std::ostream& operator << (std::ostream& o, const DVVirtualDevicePool& d){
	d.Print(o);
	return o;
}

}	//	namespace Spr

#endif
