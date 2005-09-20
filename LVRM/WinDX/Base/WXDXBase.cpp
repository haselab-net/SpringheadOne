#include "WXDXBase.h"
#include <objbase.h>
namespace LVRM {
//-----------------------------------------------------------------------------
//	WXDXBase
//
WXDXBase::WXDXBase(const WXDXBase& o){
	intf = o.intf;
	if (intf) intf->AddRef();
}
WXDXBase::WXDXBase(IUnknown* i){
	intf = i;
	if (intf) intf->AddRef();
}
WXDXBase::~WXDXBase(){
	Release();
}
void WXDXBase::Release(){
	if (intf){
		int ref = intf->Release();
		TRACEALL("WXDXBase::Release ref = %d %x\n", ref, intf);
		intf = NULL;
	}
}
WXDXBase& WXDXBase::operator = (const WXDXBase& o){
	Release();
	intf = o.intf;
	if (intf) intf->AddRef();
	return *this;
}
const IID& WXDXBase::InterfaceID() const{
	return IID_IUnknown;
}
const IID& WXDXBase::ClassID() const{
	static GUID guidNULL;
	return guidNULL;
}

//-----------------------------------------------------------------------------
//	CDXComInitImp
//
CDXComInitImp* CDXComInitImp::comInitImp;
CDXComInitImp::CDXComInitImp(){
	CoInitialize(NULL);
}
CDXComInitImp::~CDXComInitImp(){
	CoUninitialize();
}
CDXComInit::CDXComInit(){
	if (!CDXComInitImp::comInitImp) CDXComInitImp::comInitImp = new CDXComInitImp;
	(UTRef<CDXComInitImp>&)(*this) = CDXComInitImp::comInitImp;
}

}	//	namespace LVRM
