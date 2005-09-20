#ifndef GRLight_H
#define GRLight_H

#include <SceneGraph/SGFrame.h>
#include <Graphics/GRVisual.h>
#include <WinBasis/WBUtility.h>

namespace Spr {;

struct GRLightData{
    Vec4f diffuse;		///<	�g�U��
    Vec4f specular;		///<	
    Vec4f ambient;
    Vec4f position;		///<	�����̈ʒu�Cw=0�Ƃ���Ɩ�����(��������)�ɂȂ�D
    float range;		///<	�����͂��͈�
    ///@name	�������DAtten = 1/( att0 + att1 * d + att2 * d^2)
	//@{
	float attenuation0;	///<	att0	0..��
    float attenuation1;	///<	att1	0..��
    float attenuation2;	///<	att2	0..��
	//@}
	Vec3f spotDirection;///<	�X�|�b�g���C�g�̏ꍇ�̌���
    float spotFalloff;	///<	�����̑���(�傫���قǋ}�s)		0..��
	float spotInner;	///<	�X�|�b�g���C�g�̒��S����		0..spotCutoff
	float spotCutoff;	///<	�X�|�b�g���C�g�̌���������͈�	0..��
	GRLightData();
};
///
class SPR_DLL GRLight:public GRVisual, public GRLightData{
public:
	SGOBJECTDEF(GRLight);
	///	�p���t���[��
	UTRef<SGFrame> frPosture;
	/// ���[�h��ɌĂяo�����
	virtual void Loaded(SGScene* scene);
	///	�����_�����O
	virtual void Render(SGFrame* n, GRRender* render);
	virtual void Rendered(SGFrame* n, GRRender* render);
		
	virtual bool AddChildObject(SGObject* obj, SGScene* s);
	virtual size_t NReferenceObjects();
	virtual SGObject* ReferenceObject(size_t i);
	
};



}	//	Spr
#endif
