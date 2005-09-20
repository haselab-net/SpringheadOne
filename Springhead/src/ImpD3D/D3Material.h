#ifndef D3MATERIAL_H
#define D3MATERIAL_H

#include "Graphics/GRVisual.h"
#include "Graphics/GRMaterial.h"
#include <WinDX/D3D/WXD3D.h>

namespace Spr {;
/**	�}�e���A��	*/
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
	///	�}�e���A��
	D3DMATERIAL_SPR material;
	///	�e�N�X�`��
	WXINTFV(Direct3DTexture) texture;
	UTString textureFilename;
	///	�s�������H
	bool bOpaque;
	///	�����_�����O
	virtual void Render(SGFrame* n, GRRender* render);
	void SetXMaterial(XMaterial xmat);
	XMaterial GetXMaterial();
};

}	//	Spr
#endif
