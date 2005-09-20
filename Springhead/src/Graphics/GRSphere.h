#ifndef GRSPHERE_H
#define GRSPHERE_H

#include "GRVisual.h"
#include "GRMaterial.h"

namespace Spr {;

///	球プリミティブ．描画はされない．
class SPR_DLL GRSphere:public SGObject{
public:
	float radius;								///<	半径
	DWORD slices;								///<	経線方向の分割数
	DWORD stacks;								///<	緯線方向の分割数
	UTRef<GRMaterial> material;					///<	マテリアル
	SGOBJECTDEF(GRSphere);						///<
	///	材質を追加
	virtual bool AddChildObject(SGObject* obj, SGScene* s);
	///	所有しているオブジェクトの数
	virtual size_t NChildObjects();
	///	所有しているオブジェクト
	virtual SGObject* ChildObject(size_t i);
};

}
#endif
