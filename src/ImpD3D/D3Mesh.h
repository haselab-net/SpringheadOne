#ifndef D3MESH_H
#define D3MESH_H

#include <Graphics/GRMesh.h>
#include "D3Material.h"
#include <WinDX/WinDX.h>

namespace Spr {;

/**	Direct3D�ɂ�郁�b�V���̎���	*/
class SPR_DLL D3Mesh:public GRVisual{
public:
	SGOBJECTDEF(D3Mesh);
	std::vector< UTRef<D3Material> > materials;	//	�g�p����}�e���A��
	WXINTF(D3DXMesh) intf;
	///	GRMesh �ŏ�����
	void Set(GRMesh* gm, SGScene* s);
	///	�����_�����O
	virtual void Render(SGFrame* n, GRRender* render);
	//	DirectX�o�[�W�����Ⴂ�̋z��
	HRESULT LockVertexBuffer(DWORD flg, void** v);
	HRESULT LockIndexBuffer(DWORD flg, void** v);
};


}
#endif
