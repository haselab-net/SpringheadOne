#ifndef HDSPIDARI7_H
#define HDSPIDARI7_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-I
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/
class SPR_DLL HISpidarI7: public HISpidarG6{
public:
	///	�f�o�C�X�̃^�C�v
	HIOBJECTDEF(HISpidarI7);

	HISpidarI7();
	virtual bool Init(DVDeviceManager& dev);
};

}	//	namespace
#endif
