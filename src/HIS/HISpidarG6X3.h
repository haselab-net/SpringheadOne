#ifndef HDSPIDARG6X3_H
#define HDSPIDARG6X3_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G XType
(�����ł��Ď�y�Ɏ����^�ׂ�R���p�N�g�Ȃr�o�h�c�`�q�Ƃ����R���Z�v�g)
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/
class SPR_DLL HISpidarG6X3: public HISpidarG6{
public:
	HISpidarG6X3();
	bool Init(DVDeviceManager& dev, bool bLeft);

	///	�f�o�C�X�̃^�C�v
	HI_DEVICE_TYPE_DEF("SpidarG6X3")
};

}	//	namespace Spr
#endif
