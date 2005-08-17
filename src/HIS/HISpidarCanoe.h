#ifndef HDSPIDARCANOE_H
#define HDSPIDARCANOE_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/
class SPR_DLL HISpidarCanoe: public HISpidarG6{
public:
	///	�f�o�C�X�̃^�C�v
	HIOBJECTDEF(HISpidarCanoe);

	HISpidarCanoe();
	bool Init(DVDeviceManager& dev);
	virtual void Update(float dt);
	void MakeWireVec();

//	virtual ~HISpidarCanoe();
};

}	//	namespace Spr
#endif
