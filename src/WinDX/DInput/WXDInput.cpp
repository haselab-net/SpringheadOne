#include "WXDInput.h"
#pragma hdrstop

namespace Spr{;

WXDInputDevice::WXDInputDevice(){
	ZeroMemory(&info, sizeof(info));
}
WXDInputDevice::WXDInputDevice(const DIDEVICEINSTANCE& di):info(di){
}
void WXDInputDevice::Print(std::ostream& os) const{
	os << info.tszInstanceName << ":\t" << info.tszProductName;
}
bool WXDInputDevice::Create(){
	return false;
}

IUnknown* WXDInput::Create(){
	WXCHECK( DirectInput8Create(
		GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&Intf(), NULL ) );
	return Intf();
}
WXDInput::WXDInput(){
	Prepare();
}
void WXDInput::Enum(DWORD devType, DWORD flag){
	WXCHECK(Intf()->EnumDevices(devType, EnumFunc, this, flag));
}

int WXDInput::EnumFunc(const DIDEVICEINSTANCE* di, void* pRef){
	WXDInput* dinput = (WXDInput*)pRef;
	dinput->devices.push_back(*di);
	return DIENUM_CONTINUE;
}

}	//	namespace Spr
