#ifndef LVRM_DSHOW_WXAMFILTERBASE_H
#define LVRM_DSHOW_WXAMFILTERBASE_H
#include "WXAMPin.h"
#include <vector>

namespace LVRM {;

class WXAMFilterBase:public WXInterface<IBaseFilter, &IID_IBaseFilter>{
public:
	class WXAMPins:public std::vector<WXAMPin>{
	protected:
		WXAMFilterBase* filter;
	public:
		WXAMPins(){ filter = NULL; }
		void Reset();
		void Dump(std::ostream& ostr);
		friend class WXAMFilterBase;
	};
protected:
	wchar_t name[128];
public:
	WXAMPins pins;
	WXAMFilterBase(IBaseFilter* i=NULL);
	WXAMFilterBase(const WXAMFilterBase& f);
	virtual const wchar_t* Name();
	void Dump(std::ostream& ostr);
};

}	//	namespace LVRM;

#endif
