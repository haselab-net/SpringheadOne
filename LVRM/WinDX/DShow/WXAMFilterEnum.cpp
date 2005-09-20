#include "WXAMFilterEnum.h"

namespace LVRM {;
WXAMEnumedFilter::WXAMEnumedFilter(){
	name[0] = 0;
}
WXAMEnumedFilter::WXAMEnumedFilter(const WXAMEnumedFilter& f):WXAMFilterBase(f){
	moniker = f.moniker;
	wcscpy(name, f.name);
}
const wchar_t* WXAMEnumedFilter::Name(){
	WXAMFilterBase::Name();
	if (name[0] == 0){
		IPropertyBag *pBag = NULL;
		HRESULT hr = moniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr)){
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) {
				wcscpy(name, var.bstrVal);
				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
	}
	return name;
}
bool WXAMEnumedFilter::Create(){
	HRESULT hr = moniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&Intf());
	return hr == NOERROR;
}

//----------------------------------------------------------------------------
bool WXAMFilterEnum::Enum(const GUID& category){
    ICreateDevEnum *createDevEnum = NULL;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&createDevEnum);
    if (hr != NOERROR){
		DPF("Fail to create the enumerater of the video input device.\n");
		return false;
	}
	IEnumMoniker *enumMoniker = NULL;
    hr = createDevEnum->CreateClassEnumerator(category, &enumMoniker, 0);
    createDevEnum->Release();
    if (hr != NOERROR){
		DPF("Fail to create the enumMoniker.\n");
		return false;
	}
	//	デバイスの列挙
    enumMoniker->Reset();
    ULONG nDevices;
    IMoniker *moniker;
    for (int i=0; hr = enumMoniker->Next(1, &moniker, &nDevices), hr==S_OK; i++){
		WXAMEnumedFilter filter;
		filter.moniker.Intf() = moniker;
		push_back(filter);
	}
	enumMoniker->Release();
	return true;
}

}	//	namespace LVRM;
