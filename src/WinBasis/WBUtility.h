#ifndef WBUTILITY_H
#define WBUTILITY_H
#include "WBWin.h"
#include <Base/BaseUtility.h>

namespace Spr {;
class SPR_DLL WBGuid: public GUID{
public:
	WBGuid(UTString s);
	WBGuid(const GUID& g):GUID(g){}
	WBGuid(){}
	WBGuid& operator =(const GUID& g){
		*this=WBGuid(g);
		return *this;
	}
	void Print(std::ostream& os) const;
};
inline bool operator < (const WBGuid& g1, const WBGuid& g2){
	return memcmp(&g1, &g2, 16) < 0;
}
inline std::ostream& operator << (std::ostream& os, const WBGuid& g){
	g.Print(os);
	return os;
}

}
#endif