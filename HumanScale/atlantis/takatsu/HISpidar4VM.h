#ifndef HISPIDAR4VM_H
#define HISPIDAR4VM_H

#include "HISpidar4.h"

namespace Spr {;

/**	Spidar-H
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/
class SPR_DLL HISpidar4VM: public HISpidar4{
public:
	///	�f�o�C�X�̃^�C�v
	HI_DEVICE_TYPE_DEF("Spidar4VM")

	HISpidar4VM();
	virtual bool Init(DVDeviceManager& dev);
};

}	//	namespace
#endif
