#ifndef GRSPHERE_H
#define GRSPHERE_H

#include "GRVisual.h"
#include "GRMaterial.h"

namespace Spr {;

///	���v���~�e�B�u�D�`��͂���Ȃ��D
class SPR_DLL GRSphere:public SGObject{
public:
	float radius;								///<	���a
	DWORD slices;								///<	�o�������̕�����
	DWORD stacks;								///<	�ܐ������̕�����
	UTRef<GRMaterial> material;					///<	�}�e���A��
	SGOBJECTDEF(GRSphere);						///<
	///	�ގ���ǉ�
	virtual bool AddChildObject(SGObject* obj, SGScene* s);
	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NChildObjects();
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ChildObject(size_t i);
};

}
#endif
