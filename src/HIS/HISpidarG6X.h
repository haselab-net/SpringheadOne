#ifndef HDSPIDARG6X_H
#define HDSPIDARG6X_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G XType
(�����ł��Ď�y�Ɏ����^�ׂ�R���p�N�g�Ȃr�o�h�c�`�q�Ƃ����R���Z�v�g)
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/
class SPR_DLL HISpidarG6X: public HISpidarG6{
public:
	HISpidarG6X();
	bool Init(DVDeviceManager& dev);

	///	�f�o�C�X�̃^�C�v
	HI_DEVICE_TYPE_DEF("SpidarG6X")
};

}	//	namespace Spr
#endif
