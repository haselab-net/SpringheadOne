#include "WXDDMode.h"
namespace Spr {
bool WXDDDisplayMode::operator < (const WXDDDisplayMode& m) const
	{
    if (desc.bitPerPixel < m.desc.bitPerPixel) return true;
    if (desc.Size().cx < m.desc.Size().cx) return true;
    if (desc.Size().cy < m.desc.Size().cy) return true;
	return false;
	}
bool WXDDDisplayMode::operator == (const WXDDDisplayMode& m) const
	{
	return !(m < *this) && !(*this < m);
	}


WXDDDisplayModes::WXDDDisplayModes()
	{
	clear();
	}
int WXDDDisplayModes::Find(int w, int h, UINT bpp)
	{
	WXDDDisplayModes& modes = *this;
	for(DWORD i=0; i<modes.size(); i++)
		{
		if (modes[i].desc.Size().cx == w
			&& modes[i].desc.Size().cy == h
			&& modes[i].desc.bitPerPixel == bpp) return i;
		}
	return -1;
	}
}	//	namespace Spr
