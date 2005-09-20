#ifndef LVRM_DDRAW_HDDMODE_H
#define LVRM_DDRAW_HDDMODE_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WXDDSurf.h"
#include <algorithm>

namespace LVRM {
class WXDDDisplayMode
	{
	public:
	WXDDSurfaceDesc desc;
	bool operator < (const WXDDDisplayMode&) const;
	bool operator == (const WXDDDisplayMode&) const;
	};

class WXDDDisplayModes :public std::vector<WXDDDisplayMode>{
	public:
	WXDDDisplayModes();
	int Find(int w, int h, UINT bpp);
    iterator Find(const WXDDDisplayMode& key){
		return std::find(begin(), end(), key);
	}
};
}	//	namespace LVRM

#endif