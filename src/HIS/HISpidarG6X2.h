#ifndef HDSPIDARG6X2_H
#define HDSPIDARG6X2_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G XType
(�����ł��Ď�y�Ɏ����^�ׂ�R���p�N�g�Ȃr�o�h�c�`�q�Ƃ����R���Z�v�g)
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/
class SPR_DLL HISpidarG6X2: public HISpidarG6{
public:
	HISpidarG6X2();
	bool Init(DVDeviceManager& dev);

	///	�f�o�C�X�̃^�C�v
	HI_DEVICE_TYPE_DEF("SpidarG6X2")
};

}	//	namespace Spr
#endif
