#include "WXAMFilterBase.h"
namespace LVRM {;

void WXAMFilterBase::WXAMPins::Reset(){
	clear();
	IEnumPins* ep;
	(*filter)->EnumPins(&ep);
	IPin* p[256];
	ULONG nP;
	ep->Next(sizeof(p) / sizeof(p[0]), p, &nP);
	for(DWORD i=0; i<nP; i++){
		push_back(WXAMPin(p[i]));
		p[i]->Release();
	}
	ep->Release();
}
void WXAMFilterBase::WXAMPins::Dump(std::ostream& ostr){
	for(iterator it = begin(); it != end(); ++it){
		it->Dump(ostr);
	}
}
//----------------------------------------------------------------------------
WXAMFilterBase::WXAMFilterBase(IBaseFilter* i)
:WXInterface<IBaseFilter, &IID_IBaseFilter>(i){
	wcscpy(name, L"WXAMFilterBase");
	pins.filter = this;
	if (Intf()) pins.Reset();
}
WXAMFilterBase::WXAMFilterBase(const WXAMFilterBase& f)
:WXInterface<IBaseFilter, &IID_IBaseFilter>(f){
	wcscpy(name, f.name);
	pins = f.pins;
	pins.filter = this;
}
const wchar_t* WXAMFilterBase::Name(){
	if(Intf()){
		FILTER_INFO info;
		Intf()->QueryFilterInfo(&info);
		if (info.pGraph){
			wcscpy(name, info.achName);
			info.pGraph->Release();
		}
	}
	return name;
}
void WXAMFilterBase::Dump(std::ostream& ostr){
	char buf[1024];	
	wcstombs(buf, Name(), sizeof(buf));
	DPF("%s\n", buf);
    ostr << "test\n";
	ostr << buf	<< std::endl;
	pins.Dump(ostr);
}

}	//	namespace LVRM;
