#ifndef Spr_WXD3AFFINE_H
#define Spr_WXD3AFFINE_H

#include <Base/Affine.h>
#include <d3drm.h>

namespace Spr {

typedef Vec2f WXD3Vec2;

class SPR_DLL WXD3Vec3:public Vec3f{
public:
	typedef Vec3f base_type;
	DEF_TVECTOR_BASIC_MEMBER(WXD3Vec3);
	WXD3Vec3 (element_type xi, element_type yi, element_type zi){ X() = xi; Y() = yi; Z() = zi; }
	WXD3Vec3(const D3DVECTOR& v):Vec3f(v.x, v.y, v.z){}
	operator D3DVECTOR&(){ return (D3DVECTOR&)*this; }
};

class SPR_DLL WXD3Affine:public Affinef{
public:
	typedef WXD3Affine base_type;
	DEF_TMATRIX_BASIC_MEMBER(WXD3Affine);
	///	íPà çsóÒ
	WXD3Affine(){}
	operator D3DMATRIX*(){ return (D3DMATRIX*) this; }

	typedef float FLOAT4[4];
    operator FLOAT4*() { return (FLOAT4*)this; }
};

}	//	namespace Spr

#endif
