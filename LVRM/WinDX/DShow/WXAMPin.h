#ifndef LVRM_DSHOW_WXAMPIN_H
#define LVRM_DSHOW_WXAMPIN_H

#include <WinDX/Base/WxComBase.h>
#include <strmif.h>

namespace LVRM {;

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

}	//	namespace LVRM;

#endif
