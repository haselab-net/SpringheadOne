#ifndef FWAPPD3D_H
#define FWAPPD3D_H
#include "FWApp.h"
#include "FWFileIOD3D.h"

namespace Spr{;

class FWAppD3D: public FWApp{
public:
	FWFileIOD3D fileIO;
	virtual void LoadImp(UTString fn);
	virtual void SaveImp(UTString fn);
	void Init(HWND hw);
};

}
#endif
