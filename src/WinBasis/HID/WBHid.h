#ifndef WBHID_H
#define WBHID_H

#include <WinBasis/WBUtility.h>
extern "C" {
#include "hidsdi.h"
#include "hidpi.h"
#include "SETUPAPI.H"
}

namespace Spr{;



/*	USB��HID�N���X�̃f�o�C�X�̏��𒼐ڎ󂯎��d�g�݁B
	Windows2000/XP �ł́AOS���}�E�X���L���邽�߁A�}�E�X�̏��͎��Ȃ��B
	XP�ł́A����Ɂ@Raw Input API ���g�p����B*/
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