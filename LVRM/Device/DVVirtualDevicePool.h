#ifndef VR_DVDEVICES_H
#define VR_DVDEVICES_H
#include "DVVirtualDevice.h"
#include <vector>

namespace LVRM {

//----------------------------------------------------------------------------
//	DVVirtualDevices

/**	���z�f�o�C�X�̃v�[��. ISA�J�[�h��PC�J�[�h�Ȃǂ̃f�o�C�X�́C
	�@�\(D/A,�J�E���^�Ȃ�)�ʂɕ������C���z�f�o�C�X�Ƃ��Ă���
	�v�[���ɓo�^�����D
	�o�^���ꂽ�f�o�C�X�́CSPIDAR�Ȃǂ̃V�X�e���ɗ��p�����D	*/

class DVVirtualDevicePool: std::vector< UTRef<DVVirtualDevice> >{
public:
	///	Rent device.
	DVVirtualDevice* Rent(const char* type, const char* name=NULL);
	///	Rent device.
	template <class T> bool Rent(T*& t){
		t = (T*)Rent(T::TypeS());
		return t != NULL;
	}
	///	return device.
	bool Return(DVVirtualDevice* dev);
	///	Register virutal device.
	void Register(DVVirtualDevice* dev){ push_back(dev); }
	///	�_���v�o��
	void Print(std::ostream& o) const;
};
inline std::ostream& operator << (std::ostream& o, const DVVirtualDevicePool& d){
	d.Print(o);
	return o;
}

}	//	namespace LVRM

#endif
