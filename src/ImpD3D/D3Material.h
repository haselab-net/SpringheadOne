#ifndef D3MATERIAL_H
#define D3MATERIAL_H

#include "Graphics/GRVisual.h"
#include "Graphics/GRMaterial.h"
#include <WinDX/D3D/WXD3D.h>

namespace Spr {;
/**	マテリアル	*/
class SPR_DLL D3Material:public GRVisual{
public:
	struct ColorRGB{
		float r, g, b;
	};
	struct ColorRGBA{
		float r, g, b, a;
	};
	struct XMaterial{
		ColorRGBA face;
		float power;
		ColorRGB specular;
		ColorRGB emissive;
	};
	SGOBJECTDEF(D3Material);
	D3Material();
	D3Material(GRMaterial& gm);
	///	マテリアル
	D3DMATERIAL_SPR material;
	///	テクスチャ
	WXINTFV(Direct3DTexture) texture;
	UTString textureFilename;
	///	不透明か？
	bool bOpaque;
	///	レンダリング
	virtual void Render(SGFrame* n, GRRender* render);
	void SetXMaterial(XMaterial xmat);
	XMaterial GetXMaterial();
};

}	//	Spr
#endif
