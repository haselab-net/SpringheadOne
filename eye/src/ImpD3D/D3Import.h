#ifndef D3IMPORT_H
#define D3IMPORT_H

#include <SceneGraph/SGFrame.h>

namespace Spr{;

class D3Import: public SGFrame{
public:
	SGOBJECTDEF(D3Import);
	UTString filename;
	D3Import(){}
	D3Import(UTString fn):filename(fn){}
};


}

#endif
