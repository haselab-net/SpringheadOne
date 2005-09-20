#ifndef GRMATERIAL_H
#define GRMATERIAL_H

#include "Graphics/GRVisual.h"

namespace Spr {;

struct GRMaterialData{
	Vec4f diffuse;
	Vec4f ambient;
	Vec4f specular;
	Vec4f emissive;
	float power;
	GRMaterialData(){ power = 0.0f; }
	GRMaterialData(Vec4f d, Vec4f a, Vec4f s, Vec4f e, float p):
		diffuse(d), ambient(a), specular(s), emissive(e), power(p){}
	GRMaterialData(Vec4f c, float p):
		diffuse(c), ambient(c), specular(c), emissive(c), power(p){}
	GRMaterialData(Vec4f c):
		diffuse(c), ambient(c), specular(c), emissive(c), power(0.0f){}
	bool IsOpaque(){
		return diffuse.W() >= 1.0 && ambient.W() >= 1.0 && specular.W() >= 1.0 && emissive.W() >= 1.0;
	}
};
/**	マテリアル	*/
class SPR_DLL GRMaterial:public GRVisual, public GRMaterialData{
public:
	SGOBJECTDEFABST(GRMaterial);
	///	テクスチャ
	UTString textureFilename;
	///	描画
	virtual void Render(SGFrame* n, GRRender* r);
};

}	//	Spr
#endif
