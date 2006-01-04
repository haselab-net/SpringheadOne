#ifndef D3MESH_H
#define D3MESH_H

#include <Graphics/GRMesh.h>
#include "D3Material.h"
#include <WinDX/WinDX.h>

namespace Spr {;

/**	Direct3Dによるメッシュの実装	*/
class SPR_DLL D3Mesh:public GRVisual{
public:
	SGOBJECTDEF(D3Mesh);
	std::vector< UTRef<D3Material> > materials;	//	使用するマテリアル
	WXINTF(D3DXMesh) intf;
	///	GRMesh で初期化
	void Set(GRMesh* gm, SGScene* s);
	///	レンダリング
	virtual void Render(SGFrame* n, GRRender* render);
	//	DirectXバージョン違いの吸収
	HRESULT LockVertexBuffer(DWORD flg, void** v);
	HRESULT LockIndexBuffer(DWORD flg, void** v);
};


}
#endif
