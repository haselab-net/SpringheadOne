#ifndef FWFILEIOD3D_H
#define FWFILEIOD3D_H
#include "FWFileIO.h"

namespace Spr{;
class FILoadScene;
class FISaveScene;

/**	�t�@�C�����o�́D
	Direct3D�ŕ`�悷�邽�߂̃V�[���O���t��X�t�@�C��������D*/
class FWFileIOD3D{
protected:
	UTRef<FILoadScene> loader;
	UTRef<FISaveScene> saver;
public:
	///	�t�@�C�������[�h���Cscene�Ɋ܂܂��fr�̎q�Ƃ���D
	virtual bool Load(const char* fn, SGScene* scene, SGObject* obj=0);
	///	scene�Ɋ܂܂��fr���t�@�C���ɃZ�[�u����D
	virtual bool Save(const char* fn, SGScene* scene, SGObject* obj=0);
protected:
	virtual bool PrepareLoader();
	virtual bool PrepareSaver();
};

}
#endif
