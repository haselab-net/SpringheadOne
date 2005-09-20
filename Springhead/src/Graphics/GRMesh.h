#ifndef GRMESH_H
#define GRMESH_H
#include "GRVisual.h"
#include "GRMaterial.h"

namespace Spr {;

/**	���b�V���f�[�^�ێ��N���X�D
	Material���Q�Ƃ���̂ŁCGR�ɓ���Ă��邪�CVisual�ł͂Ȃ��D*/
class SPR_DLL GRMesh:public SGObject{
public:
	SGOBJECTDEF(GRMesh);
	std::vector< UTRef<GRMaterial> > materials;	///<	�}�e���A��
	std::vector<Vec3f> vertices;				///<	���_��
	std::vector<Vec3f> normals;					///<	�@����
	std::vector<Vec2f> texCoords;				///<	�e�N�X�`�����W
	std::vector<DWORD> triangles;				///<	3�p�`
	std::vector<DWORD> attributes;				///<	���_�̑���
	///	�ގ���ǉ�
	virtual bool AddChildObject(SGObject* obj, SGScene* s);
	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NChildObjects();
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ChildObject(size_t i);
};

}
#endif
