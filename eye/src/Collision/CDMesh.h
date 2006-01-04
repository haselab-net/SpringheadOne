#ifndef CDMESH_H
#define CDMESH_H
#include "CDConvex.h"

namespace Spr {;

/**	���b�V��	*/
class CDMesh:public CDGeometry{
	SGOBJECTDEF(CDMesh);
public:
	///	GeomtryID
	virtual int GeometryID(){ return MESH; }
	///	���_�̍��W(���[�J���n)
	std::vector<Vec3f> vertices;
	///	���_�̍��W(��Ɨ̈�)
	std::vector<Vec3f> tvtxs;
	///	�ʌ`��	�{���� Tree �ɂ������D
	CDGeometries conveces;
	///	�c���[�̍쐬
	void CreateTree();
	///
	void MergeVertices();
	///
	virtual void CalcBBox(Vec3f& bbMin, Vec3f& bbMax);
	///	
	virtual bool AddChildObject(SGObject* o, SGScene* s);
};



}
#endif
