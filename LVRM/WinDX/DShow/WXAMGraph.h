#ifndef LVRM_DSHOW_WXAMGRAPH_H
#define LVRM_DSHOW_WXAMGRAPH_H
#include <WinDX/Base/WxComBase.h>
#include "WXAMFilterBase.h"
#include <control.h>



namespace LVRM {;

class WXAMGraph:public WXInterface<IGraphBuilder, &IID_IGraphBuilder>{
	WXComInit comInit;
public:
	class WXAMFilters: public std::vector<WXAMFilterBase>{
	protected:
		WXAMGraph* graph;
		WXAMFilters();
	public:
		void Reset();
		void Dump(std::ostream& ostr);
		friend class WXAMGraph;
	} filters;
	WXInterface<IMediaControl, &IID_IMediaControl> mediaControl;
	WXAMGraph(IGraphBuilder* i=NULL);
	WXAMGraph(const WXAMGraph& g);
	virtual ~WXAMGraph();
	//	Add filters and connect it to any vacant pin.
	void Join(WXAMFilterBase& filter){
		Join(filter, filter.Name());
	}
	//	Add filters and connect it to any vacant pin.
	void Join(IBaseFilter* filter, const wchar_t* name);
	//	Add filters and connect them.
	bool Connect(IBaseFilter* outFilter, const wchar_t* outName, IBaseFilter* inFilter, const wchar_t* inName);
	//	Add filters and connect them.
	bool Connect(WXAMFilterBase& outFilter, WXAMFilterBase& inFilter){
		return Connect(outFilter, outFilter.Name(), inFilter, inFilter.Name());
	}
	//	Add filters and connect them with specified pin.
	bool Connect(WXAMPin& out, const wchar_t* outName, WXAMPin& in, const wchar_t* inName);
	//	Disconnect all pins;
	void Disconnect();
	//	connect filters for rendering a filter.
	bool Render(IBaseFilter* out);
	//	Run filter graph.
	void Run();
	//	Pause filter graph.
	void Pause();
	//	Stop filter graph.
	void Stop();
	//	Dump filter graph.
	void Dump(std::ostream& ostr);
protected:
	bool AddFilter(IBaseFilter* addf, const wchar_t* name=NULL);
	void WaitForState(OAFilterState state);
};

}	//	namespace LVRM;

#endif
