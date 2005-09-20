#include "WXAMGuidInfo.h"
#include <errors.h>
#include <memory.h>
#include <strstream>

namespace LVRM{

const WXAMGuidInfo *pSubTypes[] = {
    audiosubtypes,      // Analog audio
    analogvideosubtypes,// Analog video
    audiosubtypes,      // Audio
    line21subtypes,     // Line21 data
    mpeg2subtypes,      // MPEG-2.  Used by DVD.
    streamsubtypes,     // Byte stream with no time stamps
    videosubtypes,      // Video
};

const char* GetTextForMajorType(const GUID& guid){
	for(int i=0; i<sizeof(majortypes)/sizeof(majortypes[0]); i++){
		if (memcmp(majortypes[i].pGuid, &guid, sizeof(guid)) == 0){
			return majortypes[i].name;
		}
	}
	static char buf[1024];
	strcpy(buf, "Unknown(");
	std::ostrstream(buf, sizeof(buf)) << "Unknown(" << guid.Data1
		<< " " << guid.Data2 << " " << guid.Data3 << " " << *(DWORD*)&guid.Data4 << ")";
	return buf;
}
const char* GetTextForSubType(const GUID& guidMajor, const GUID& guidSub){
	return "";
}

WXAMError& WXAMError::Error(){
	static WXAMError error;
	return error;
}

bool WXAMError::Check(DWORD n, const char* msg, const char* fn, int ln){
	if (n==S_OK) return false;
	if (n==0) return false;
	AMErrorToString(err, desc, n);
	WXDXError::Check(n, msg, fn, ln);
	return true;
}
bool WXAMError::AMErrorToString(char*& err, char*& desc, HRESULT error){
	static char buf[4096];
	AMGetErrorText(error, buf, sizeof(buf));
	if(strlen(buf)){
		err = buf;
		desc = "";
		return true;
	}
	return false;
}

}
