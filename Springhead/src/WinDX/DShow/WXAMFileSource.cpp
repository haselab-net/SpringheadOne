#include "WXAMFileSource.h"

#include <uuids.h>
#include <wchar.h>
#include <stdio.h>

namespace Spr {;

WXAMFileSource::WXAMFileSource(){
	CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_ALL, IID_IFileSourceFilter, (void**)&fileSourceFilter.Intf());
	fileSourceFilter.Intf()->QueryInterface(IID_IBaseFilter, (void**)&Intf());
}
bool WXAMFileSource::Load(const char* filename){
	WCHAR wcFileName[1024];
#ifdef UNICODE
	lstrcpy(wcFileName, filename);
#else
	swprintf(wcFileName, L"%hs", filename);
#endif
	return fileSourceFilter->Load(wcFileName, NULL) == NOERROR;
}

}	//	namespace Spr;
