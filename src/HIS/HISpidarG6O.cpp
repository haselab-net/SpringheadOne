#include "HIS.h"
#pragma hdrstop
#include "HISpidarG6O.h"

namespace Spr {;
//----------------------------------------------------------------------------
HISpidarG6O::HISpidarG6O() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/

bool HISpidarG6O::Init(DVDeviceManager& dev){
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	const float PX = 0.213f/2;		//	x�����̕ӂ̒���/2
	const float PY = 0.214f/2;		//	y�����̕ӂ̒���/2
	const float PZ = 0.170f/2;		//	z�����̕ӂ̒���/2
	//	���̃O���b�v�ւ̎��t���ʒu
	const float GX = 0.065f/2;		//	x�����̕ӂ̒���/2
	const float GY = 0.065f/2;		//	y�����̕ӂ̒���/2

	Vec3f motorPos[8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f(-PX,-PY,-PZ), Vec3f(0.0f,  -GY, 0.0f)},
		{Vec3f(-PX,-PY, PZ), Vec3f( -GX, 0.0f, 0.0f)},
		{Vec3f( PX,-PY, PZ), Vec3f(  GX, 0.0f, 0.0f)},
		{Vec3f( PX,-PY,-PZ), Vec3f(0.0f,  -GY, 0.0f)},
		{Vec3f(-PX, PY,-PZ), Vec3f(0.0f,   GY, 0.0f)},
		{Vec3f(-PX, PY, PZ), Vec3f( -GX, 0.0f, 0.0f)},
		{Vec3f( PX, PY, PZ), Vec3f(  GX, 0.0f, 0.0f)},
		{Vec3f( PX, PY,-PZ), Vec3f(0.0f,   GY, 0.0f)}
	};
/*
	//�L�����u���[�V�����ʒu�𒆐S���炸�炷
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/
	if( HISpidarG6::Init(dev, 8, motorPos, 0.15f, 2.03086921e-5f, 1.0f, 20.0f) == false ){
		return false;
	}
	motor[0].lengthPerPulse *= -1;
	motor[1].lengthPerPulse *= -1;
	motor[5].lengthPerPulse *= -1;
	motor[6].lengthPerPulse *= -1;
	motor[7].lengthPerPulse *= -1;
	Calib();
	return true;
}

}//	namespace Spr
