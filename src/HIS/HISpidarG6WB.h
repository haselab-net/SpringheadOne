#ifndef HDSPIDARG6WB_H
#define HDSPIDARG6WB_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/
class SPR_DLL HISpidarG6WB: public HISpidarG6{
public:
	///	�f�o�C�X�̃^�C�v
	HI_DEVICE_TYPE_DEF("SpidarG6BW")

	HISpidarG6WB();
	bool Init(DVDeviceManager& dev);

//	virtual ~HISpidarG6WB();
};

}	//	namespace Spr
#endif
