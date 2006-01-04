#include "Collision.h"
#pragma hdrstop

namespace Spr {;
//------------------------------------------------------------------------
SGOBJECTIMP(CDSphere, CDConvex);
///	�T�|�[�g�|�C���g�����߂�D
Vec3f CDSphere::Support(const Vec3f& p_) const{
	Vec3f p = p_;
	p -= center;
	float s = p.norm();
	if (s > 1e-8f) {
		float r = radius / s;
		return r*p + center;
	}else{
		return center;
	}
}
void CDSphere::CalcBBox(Vec3f& bbMin, Vec3f& bbMax){
	bbMin.element_min(center - Vec3f(radius,radius,radius));
	bbMax.element_max(center + Vec3f(radius,radius,radius));
}

/**	���v���~�e�B�u���h�L�������g���烍�[�h*/
class CDSphereLoader:public FIObjectLoader<CDSphere>{
public:
	virtual bool LoadData(FILoadScene* ctx, TLoaded* sph){
		ctx->docs.Top()->GetData(sph->radius, "Radius");
		return true;
	}
};
DEF_REGISTER_LOADER(CDSphere);

class CDSphereForShapeLoader: public CDSphereLoader{
	virtual UTString GetNodeType() const{
		return "SphereForShape";
	}
};
DEF_REGISTER_LOADER(CDSphereForShape);

}

