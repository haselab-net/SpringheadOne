#ifndef HDSPIDARG6JUN_H
#define HDSPIDARG6JUN_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G CType
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/
class SPR_DLL HISpidarG6Jun: public HISpidarG6{
public:
	HIOBJECTDEF(HISpidarG6Jun);
	HISpidarG6Jun();
	bool Init(DVDeviceManager& dev);
};

}	//	namespace Spr
#endif
