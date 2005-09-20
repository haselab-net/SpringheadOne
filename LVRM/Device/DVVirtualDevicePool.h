#ifndef VR_DVDEVICES_H
#define VR_DVDEVICES_H
#include "DVVirtualDevice.h"
#include <vector>

namespace LVRM {

//----------------------------------------------------------------------------
//	DVVirtualDevices

/**	仮想デバイスのプール. ISAカードやPCカードなどのデバイスは，
	機能(D/A,カウンタなど)別に分けられ，仮想デバイスとしてこの
	プールに登録される．
	登録されたデバイスは，SPIDARなどのシステムに利用される．	*/

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
	///	ダンプ出力
	void Print(std::ostream& o) const;
};
inline std::ostream& operator << (std::ostream& o, const DVVirtualDevicePool& d){
	d.Print(o);
	return o;
}

}	//	namespace LVRM

#endif
