#ifndef Spr_DSHOW_WXAMFILTERENUM_H
#define Spr_DSHOW_WXAMFILTERENUM_H
#include "WXAMFilterBase.h"
#include <uuids.h>

namespace Spr {;

class CMoniker:public WXInterface<IMoniker, &IID_IMoniker>{
};
class WXAMEnumedFilter:public WXAMFilterBase{
protected:
	wchar_t name[128];
	WXAMEnumedFilter();
public:
	CMoniker moniker;
	WXAMEnumedFilter(const WXAMEnumedFilter& f);
	virtual bool Create();
	virtual const wchar_t* Name();
	friend class WXAMFilterEnum;
};

class WXAMFilterEnum:public std::vector<WXAMEnumedFilter>{
	WXComInit comInit;
public:
	///	ICreateDevEnum ÇéQè∆
	bool Enum(const GUID& category = CLSID_VideoInputDeviceCategory);
};

}	//	namespace Spr;

#endif
