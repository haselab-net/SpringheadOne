#ifndef Spr_DSHOW_WXAMPIN_H
#define Spr_DSHOW_WXAMPIN_H

#include <WinDX/Base/WXComBase.h>
#include <strmif.h>

namespace Spr {;

class WXAMPin:public WXInterface<IPin, &IID_IPin>{
protected:
	PIN_INFO info;
public:
	WXAMPin(IPin* p=NULL);
	WXAMPin(const WXAMPin&);
	~WXAMPin();
	virtual const wchar_t* Name();
	virtual PIN_DIRECTION Dir();
	virtual bool IsConnected();
	IBaseFilter* Filter();
	void Dump(std::ostream& ostr);
};

}	//	namespace Spr;

#endif
