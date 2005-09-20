#ifndef LVRM_DSHOW_WXAMFILESOURCE_H
#define LVRM_DSHOW_WXAMFILESOURCE_H
#include "WXAMFilterBase.h"


namespace LVRM {;

class WXAMFileSource:public WXAMFilterBase{
	WXComInit comInit;
protected:
	WXInterface<IFileSourceFilter, &IID_IFileSourceFilter> fileSourceFilter;
public:
	WXAMFileSource();
	bool Load(const char* filename);
	const wchar_t* Name(){ return L"WXAMFileSource"; }
	operator IFileSourceFilter*(){ return fileSourceFilter.Intf(); }
	operator const IFileSourceFilter*() const{ return fileSourceFilter.Intf(); }
};

}	//	namespace LVRM;

#endif
