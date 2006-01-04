#include "HIS.h"
#pragma hdrstop
#include "HISpidarG6Jun.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarG6Jun, HISpidarG6);
HISpidarG6Jun::HISpidarG6Jun() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/

bool HISpidarG6Jun::Init(DVDeviceManager& dev){
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	#define PX	(0.235f/2)		//	x�����̕ӂ̒���/2
	#define PY	(0.225f/2)		//	y�����̕ӂ̒���/2
	#define PZ	(0.235f/2)		//	z�����̕ӂ̒���/2
	//	���̃O���b�v�ւ̎��t���ʒu
	#define GX	(0.0555f/2)		//	x�����̕ӂ̒���/2
	#define GY	(0.0555f/2)		//	y�����̕ӂ̒���/2
	Vec3f motorPos[8][2] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-PX, PY, PZ), Vec3f(-GX,  0, 0)},
		{Vec3f( PX, PY,-PZ), Vec3f(  0, GY, 0)},
		{Vec3f(-PX,-PY,-PZ), Vec3f(  0,-GY, 0)},
		{Vec3f( PX,-PY, PZ), Vec3f( GX,  0, 0)},
		{Vec3f( PX, PY, PZ), Vec3f( GX,  0, 0)},
		{Vec3f(-PX, PY,-PZ), Vec3f(  0, GY, 0)},
		{Vec3f( PX,-PY,-PZ), Vec3f(  0,-GY, 0)},
		{Vec3f(-PX,-PY, PZ), Vec3f(-GX,  0, 0)}
	};
	if (!HISpidarG6::Init(dev, 8, motorPos, 0.3f, 2.924062107079e-5f, 1.0f, 20.0f)){
		return false;
	}
	for(int i=4; i<8; ++i){
		motor[i].lengthPerPulse *= -1;
	}
	Calib();
	return true;
}

}//	namespace Spr
