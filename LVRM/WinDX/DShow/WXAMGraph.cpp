#include "WXAMGraph.h"
#include "WXAMGuidinfo.h"
#include <uuids.h>

namespace LVRM {;

WXAMGraph::WXAMFilters::WXAMFilters(){
	graph = NULL;
}
void WXAMGraph::WXAMFilters::Reset(){
	clear();
	IEnumFilters* filterEnum = NULL;
	graph->Intf()->EnumFilters(&filterEnum);
	IBaseFilter* filters[256];
	ULONG nFilters;
	filterEnum->Next(256, filters, &nFilters);
	filterEnum->Release();
	for(ULONG i=0; i<nFilters; i++){
		push_back(WXAMFilterBase(filters[i]));
		filters[i]->Release();
	}
}
void WXAMGraph::WXAMFilters::Dump(std::ostream& ostr){
	for(iterator it = begin(); it != end(); ++it){
		it->Dump(ostr);
	}
}

WXAMGraph::WXAMGraph(IGraphBuilder* i):WXInterface<IGraphBuilder, &IID_IGraphBuilder>(i){
	filters.graph = this;
	if (!Intf()){
		CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_ALL, IID_IGraphBuilder, (LPVOID *)&Intf());
	}
	Intf()->QueryInterface(mediaControl.InterfaceID(), (void**)&mediaControl.Intf());
}
WXAMGraph::WXAMGraph(const WXAMGraph& g):WXInterface<IGraphBuilder, &IID_IGraphBuilder>(g){
	filters.graph = this;
	Intf()->QueryInterface(mediaControl.InterfaceID(), (void**)&mediaControl.Intf());
}
WXAMGraph::~WXAMGraph(){
	Stop();
	Disconnect();
}
bool WXAMGraph::AddFilter(IBaseFilter* addf, const wchar_t* name){
	for(WXAMFilters::iterator it = filters.begin(); it != filters.end(); ++it){
		if (it->Intf() == addf) return true;
	}
	//	追加するフィルタがグラフに入ってなければ追加する．
	HRESULT hr = Intf()->AddFilter(addf, name);
	if (hr != S_OK) DPF("Fail to AddFilter.\n");
	filters.Reset();
	return hr == S_OK;
}
bool WXAMGraph::Connect(WXAMPin& out, const wchar_t* outName, WXAMPin& in, const wchar_t* inName){
	AddFilter(out.Filter(), outName);
	AddFilter(in.Filter(), inName);
	HRESULT hr = Intf()->Connect(out, in);
	filters.Reset();
	return hr == NOERROR;
}
bool WXAMGraph::Connect(IBaseFilter* outFilter, const wchar_t* outName, IBaseFilter* inFilter, const wchar_t* inName){
	if (!outFilter || !inFilter) return false;
	AddFilter(outFilter, outName);
	AddFilter(inFilter, inName);

	WXAMFilterBase out(outFilter);
	WXAMFilterBase in(inFilter);

	for(WXAMFilterBase::WXAMPins::iterator itOut = out.pins.begin(); itOut != out.pins.end(); ++itOut){
		if (itOut->Dir() == PINDIR_OUTPUT){
			if (itOut->IsConnected()) continue;
			for(WXAMFilterBase::WXAMPins::iterator itIn = in.pins.begin(); itIn != in.pins.end(); ++itIn){
				if (itIn->Dir() == PINDIR_INPUT){
					HRESULT hr = Intf()->Connect(itOut->Intf(), itIn->Intf());
					if (hr == NOERROR){
						filters.Reset();
						return true;
					}
					WXAMError::Error().CKDXERR(hr);
				}
			}
		}
	}
	return false;
}
void WXAMGraph::Join(IBaseFilter* filter_, const wchar_t* name_){
	WXAMFilterBase newFilter(filter_); 
	AddFilter(newFilter);
	filters.Reset();
	for(WXAMFilterBase::WXAMPins::iterator it = newFilter.pins.begin(); it != newFilter.pins.end(); ++it){
		WXAMPin& newPin = *it;
		if (newPin.IsConnected()) continue;
		for(WXAMFilters::iterator it = filters.begin(); it != filters.end(); ++it){
			WXAMFilterBase& filterInGraph = *it;
			if (filterInGraph == newFilter) continue;
			for(WXAMFilterBase::WXAMPins::iterator it = filterInGraph.pins.begin(); it != filterInGraph.pins.end(); ++it){
				WXAMPin& pin = *it;
				if (pin.IsConnected()) continue;
				if (newPin.Dir() == PINDIR_INPUT && pin.Dir() == PINDIR_OUTPUT){
					HRESULT hr = Intf()->Connect(pin.Intf(), newPin.Intf());
					WXAMError::Error().CKDXERR(hr);
				}else if (newPin.Dir() == PINDIR_OUTPUT && pin.Dir() == PINDIR_INPUT){
					HRESULT hr = Intf()->Connect(newPin.Intf(), pin.Intf());
					WXAMError::Error().CKDXERR(hr);
				}
			}
		}
	}
}
void WXAMGraph::Disconnect(){
	Stop();
	for(WXAMFilters::iterator it = filters.begin(); it != filters.end(); ++it){
		it->pins.Reset();
		for(WXAMFilterBase::WXAMPins::iterator pin = it->pins.begin(); pin != it->pins.end(); ++pin){
			pin->Intf()->Disconnect();
		}
	}
}
bool WXAMGraph::Render(IBaseFilter* outFilter){
	if (!outFilter) return false;
	AddFilter(outFilter, NULL);
	
	//	outFilter の Pin の列挙
	IEnumPins* outEnum = NULL;
	outFilter->EnumPins(&outEnum);
	IPin* outPins[256];
	ULONG nOutPins;
	outEnum->Next(256, outPins, &nOutPins);
	outEnum->Release();

	//	順番に繋いでみる
	bool rv = false;
	for(ULONG i=0; i<nOutPins; i++){
		PIN_DIRECTION dir;
		outPins[i]->QueryDirection(&dir);
		if (dir != PINDIR_OUTPUT) continue;
		HRESULT hr = Intf()->Render(outPins[i]);
		if (hr == NOERROR){
			rv = true;
			goto next;
		}
	}
next:	;
	for(ULONG i=0; i<nOutPins; i++) outPins[i]->Release();
	return rv;
}

void WXAMGraph::WaitForState(OAFilterState state){
	OAFilterState st;
	do{
		mediaControl->GetState(10, &st);
	} while(st != state);
}
void WXAMGraph::Pause(){
	mediaControl->Pause();
	WaitForState(State_Paused);
}
void WXAMGraph::Stop(){
	mediaControl->Stop();
	WaitForState(State_Stopped);
}
void WXAMGraph::Run(){
	mediaControl->Run();
	WaitForState(State_Running);
}
void WXAMGraph::Dump(std::ostream& ostr){
	filters.Reset();
	filters.Dump(ostr);
}

}	//	namespace LVRM;
