#ifndef WBHID_H
#define WBHID_H

#include <WinBasis/WBUtility.h>
extern "C" {
#include "hidsdi.h"
#include "hidpi.h"
#include "SETUPAPI.H"
}

namespace Spr{;



/*	USBのHIDクラスのデバイスの情報を直接受け取る仕組み。
	Windows2000/XP では、OSがマウスを占有するため、マウスの情報は取れない。
	XPでは、代わりに　Raw Input API を使用する。*/
class WBHidDevice: public UTRefCount{
public:
	SP_DEVICE_INTERFACE_DETAIL_DATA* data;
	HIDD_ATTRIBUTES attr;
	HIDP_CAPS caps;
	
	WBHidDevice(SP_DEVICE_INTERFACE_DETAIL_DATA* d=NULL):data(d){}
	~WBHidDevice(){ delete data; }
	HANDLE Create(DWORD dwDesiredAccess=0, DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE);
	void Print(std::ostream& os) const;
	UTString GetError();
};
class WBHidDevices:public std::vector< UTRef<WBHidDevice> >{
public:
};

class WBHid{
public:
	WBGuid guidHid;	
	HDEVINFO hDevInfo;
	SP_INTERFACE_DEVICE_DATA devInfoData;
	WBHidDevices devices;
	WBHid(){ Init(); }	
	void Enum();
	void Init();
};

}
#endif