#ifndef HDSPIDARG6X_H
#define HDSPIDARG6X_H

#include "HISpidarG6.h"

namespace Spr {;

/**	Spidar-G XType
(分解できて手軽に持ち運べるコンパクトなＳＰＩＤＡＲというコンセプト)
	フレームのサイズやモータの符号を含む	*/
class SPR_DLL HISpidarG6X: public HISpidarG6{
public:
	HISpidarG6X();
	bool Init(DVDeviceManager& dev);

	///	デバイスのタイプ
	HI_DEVICE_TYPE_DEF("SpidarG6X")
};

}	//	namespace Spr
#endif
