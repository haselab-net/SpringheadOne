#ifndef CDSPHERE_H
#define CDSPHERE_H
#include <Collision/CDGeometry.h>
#include <Collision/CDConvex.h>

namespace Spr {;
/**	����	*/
class CDSphere:public CDConvex{
	mutable Vec3f curPos;
public:
	///	���S�̍��W
	Vec3f center;
	///	���a
	float radius;

	SGOBJECTDEF(CDSphere);
	///
	CDSphere(float r = 0.0f, Vec3f c = Vec3f()):radius(r), center(c){}
	///	GeomtryID
	virtual int GeometryID(){ return CONVEX; }
	
	///	�T�|�[�g�|�C���g�����߂�D
	virtual Vec3f Support(const Vec3f& p) const;
	///	BBox�̌v�Z
	void CalcBBox(Vec3f& bbMin, Vec3f& bbMax);
};

}
#endif
