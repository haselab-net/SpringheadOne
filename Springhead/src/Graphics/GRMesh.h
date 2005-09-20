#ifndef GRMESH_H
#define GRMESH_H
#include "GRVisual.h"
#include "GRMaterial.h"

namespace Spr {;

/**	メッシュデータ保持クラス．
	Materialを参照するので，GRに入れてあるが，Visualではない．*/
class SPR_DLL GRMesh:public SGObject{
public:
	SGOBJECTDEF(GRMesh);
	std::vector< UTRef<GRMaterial> > materials;	///<	マテリアル
	std::vector<Vec3f> vertices;				///<	頂点列
	std::vector<Vec3f> normals;					///<	法線列
	std::vector<Vec2f> texCoords;				///<	テクスチャ座標
	std::vector<DWORD> triangles;				///<	3角形
	std::vector<DWORD> attributes;				///<	頂点の属性
	///	材質を追加
	virtual bool AddChildObject(SGObject* obj, SGScene* s);
	///	所有しているオブジェクトの数
	virtual size_t NChildObjects();
	///	所有しているオブジェクト
	virtual SGObject* ChildObject(size_t i);
};

}
#endif
