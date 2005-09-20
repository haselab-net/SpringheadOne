#include <WinDX/WinDX.h>
#pragma hdrstop

namespace Spr {
//-----------------------------------------------------------------------------
//	WXUnknown
//
WXUnknown::WXUnknown(const WXUnknown& o){
	intf = o.intf;
	if (intf) intf->AddRef();
}
WXUnknown::WXUnknown(IUnknown* i){
	intf = i;
	if (intf) intf->AddRef();
}
WXUnknown::~WXUnknown(){
	Release();
}
#ifdef __BORLANDC__
 #pragma warn -aus
#endif
void WXUnknown::Release(){
	if (intf){
		int ref = intf->Release();
		TRACEALL("WXUnknown::Release ref = %d %x\n", ref, intf);
		intf = NULL;
	}
}
#ifdef __BORLANDC__
 #pragma warn .aus
#endif
WXUnknown& WXUnknown::operator = (const WXUnknown& o){
	Release();
	intf = o.intf;
	if (intf) intf->AddRef();
	return *this;
}
const IID& WXUnknown::InterfaceID() const{
	return IID_IUnknown;
}
const IID& WXUnknown::ClassID() const{
	static GUID guidNULL;
	return guidNULL;
}

//-----------------------------------------------------------------------------
//	WXComInitImp
//
WXComInitImp* WXComInitImp::comInitImp;
WXComInitImp::WXComInitImp(){
	CoInitialize(NULL);
}
WXComInitImp::~WXComInitImp(){
	CoUninitialize();
}
WXComInit::WXComInit(){
	if (!WXComInitImp::comInitImp) WXComInitImp::comInitImp = new WXComInitImp;
	(UTRef<WXComInitImp>&)(*this) = WXComInitImp::comInitImp;
}

}	//	namespace Spr




