#ifndef D3SPHERE_H
#define D3SPHERE_H

#include "D3Mesh.h"
#include <Graphics/GRSphere.h>

namespace Spr {;

/**	Direct3Dによるメッシュの実装	*/
class SPR_DLL D3Sphere:public D3Mesh{
public:
	SGOBJECTDEF(D3Sphere);
	///
	void Set(GRSphere* s, SGScene* sc);
};


}
#endif
