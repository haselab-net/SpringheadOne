#ifndef CDGEOMETRY_H
#define CDGEOMETRY_H
#include <SceneGraph/SGObject.h>

namespace Spr {;

class CDConvex;
class CDMesh;
class CDFace;

/**	�Փ˔���Ώۂ�ID	*/
enum CDGeometryID{
	GEOMETRY=0,
	MESH,
	CONVEX,
	NGEOMETRYID
};

class PHPhysicalMaterial;

/**	�Փ˔���̂��߂̌`��f�[�^
*/
class SPR_DLL CDGeometry:public SGObject{
public:
	SGOBJECTDEFABST(CDGeometry);
	///
	virtual int GeometryID()=0;
	///
	virtual void CalcBBox(Vec3f& bbMin, Vec3f& bbMax){}
	///	�����}�e���A��
	UTRef<PHPhysicalMaterial> pmaterial;
	///	
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///
	virtual size_t NChildObjects(){ return pmaterial ? 1 : 0; }
	///
	virtual SGObject* ChildObject(size_t i);
};
/// Geometry�̔z��
class SPR_DLL CDGeometries:public std::vector< UTRef<class CDGeometry> >{
public:
};

}
#endif
