#include "WXAMPin.h"

namespace LVRM {;

WXAMPin::WXAMPin(IPin* p):WXInterface<IPin, &IID_IPin>(p){
	ZeroMemory(&info, sizeof(info));
}
WXAMPin::WXAMPin(const WXAMPin&p):WXInterface<IPin, &IID_IPin>(p){
	ZeroMemory(&info, sizeof(info));
	info.pFilter = p.info.pFilter;
	if (info.pFilter) info.pFilter->AddRef();
}
WXAMPin::~WXAMPin(){
	if (info.pFilter) info.pFilter->Release();
}
const wchar_t* WXAMPin::Name(){
	if (!info.pFilter) {
		HRESULT hr = Intf()->QueryPinInfo(&info);
		if (hr != NOERROR) info.achName[0] = 0;
	}
	return info.achName;
}
PIN_DIRECTION WXAMPin::Dir(){
	if (!info.pFilter) Intf()->QueryPinInfo(&info);
	return info.dir;
}
bool WXAMPin::IsConnected(){
	IPin* cp=NULL;
	Intf()->ConnectedTo(&cp);
	return cp != NULL;
}
IBaseFilter* WXAMPin::Filter(){
	if (!info.pFilter) Intf()->QueryPinInfo(&info);
	return info.pFilter;
}
void WXAMPin::Dump(std::ostream& ostr){
	char buf[1024];
	wcstombs(buf, Name(), sizeof(buf));
	ostr << "    " << buf << ": " << (Dir() == PINDIR_INPUT ? "in" : "out");
	IPin* ipinCon = NULL;
	Intf()->ConnectedTo(&ipinCon);
	if (ipinCon){
		WXAMPin pinCon(ipinCon);
		pinCon->Release();
		if (pinCon.Filter()){
			FILTER_INFO info;
			pinCon.Filter()->QueryFilterInfo(&info);
			info.pGraph->Release();
			char buf[1024];
			wcstombs(buf, info.achName, sizeof(buf));
			ostr << "\t-" << buf;
		}
	}
	ostr << std::endl;
}
}	//	namespace LVRM;
