#ifndef WXDINPUT_H
#define WXDINPUT_H
#include "../Base/WXComBase.h"
#include "../Base/WXError.h"

#if SPR_DXVER == 8
# define DIRECTINPUT_VERSION 0x0800
#elif SPR_DXVER == 9
# define DIRECTINPUT_VERSION 0x0900
#endif

#include <dinput.h>

namespace Spr{;

class SPR_DLL WXDInputDevice: public WXInterface<IDirectInputDevice8, &IID_IDirectInputDevice8>{
public:
	DIDEVICEINSTANCE info;
	WXDInputDevice();
	WXDInputDevice(const DIDEVICEINSTANCE& di);
	bool Create();
	void Print(std::ostream& os) const;
};
class SPR_DLL WXDInputDevices: public std::vector<WXDInputDevice>{
};

class SPR_DLL WXDInput:public WXSingletonInterface<IDirectInput8, &IID_IDirectInput8>{
	IUnknown* Create();
	static int __stdcall EnumFunc(const DIDEVICEINSTANCE* di, void* pRef);
public:
	WXDInputDevices devices;
	WXDInput();
	void Enum(DWORD devType=DI8DEVCLASS_ALL, DWORD flag=DIEDFL_ALLDEVICES);
};

}	//	namespace Spr
#endif
