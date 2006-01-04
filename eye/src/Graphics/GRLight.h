#ifndef GRLight_H
#define GRLight_H

#include <SceneGraph/SGFrame.h>
#include <Graphics/GRVisual.h>
#include <WinBasis/WBUtility.h>

namespace Spr {;

struct GRLightData{
    Vec4f diffuse;		///<	拡散光
    Vec4f specular;		///<	
    Vec4f ambient;
    Vec4f position;		///<	光源の位置，w=0とすると無限遠(方向光源)になる．
    float range;		///<	光が届く範囲
    ///@name	減衰率．Atten = 1/( att0 + att1 * d + att2 * d^2)
	//@{
	float attenuation0;	///<	att0	0..∞
    float attenuation1;	///<	att1	0..∞
    float attenuation2;	///<	att2	0..∞
	//@}
	Vec3f spotDirection;///<	スポットライトの場合の向き
    float spotFalloff;	///<	減衰の早さ(大きいほど急峻)		0..∞
	float spotInner;	///<	スポットライトの中心部分		0..spotCutoff
	float spotCutoff;	///<	スポットライトの光が当たる範囲	0..π
	GRLightData();
};
///
class SPR_DLL GRLight:public GRVisual, public GRLightData{
public:
	SGOBJECTDEF(GRLight);
	///	姿勢フレーム
	UTRef<SGFrame> frPosture;
	/// ロード後に呼び出される
	virtual void Loaded(SGScene* scene);
	///	レンダリング
	virtual void Render(SGFrame* n, GRRender* render);
	virtual void Rendered(SGFrame* n, GRRender* render);
		
	virtual bool AddChildObject(SGObject* obj, SGScene* s);
	virtual size_t NReferenceObjects();
	virtual SGObject* ReferenceObject(size_t i);
	
};



}	//	Spr
#endif
