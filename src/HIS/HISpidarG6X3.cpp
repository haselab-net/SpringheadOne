#include "HIS.h"
#pragma hdrstop
#include "HISpidarG6X3.h"

namespace Spr {;
//----------------------------------------------------------------------------
HISpidarG6X3::HISpidarG6X3() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/

bool HISpidarG6X3::Init(DVDeviceManager& dev, bool bLeft=false){
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	const float PX = 0.12f/2;		//	x�����̕ӂ̒���/2
	const float PY = 0.12f/2;		//	y�����̕ӂ̒���/2
	const float PZ = 0.12f/2;		//	z�����̕ӂ̒���/2
	//	���̃O���b�v�ւ̎��t���ʒu�D�O���b�v�̓s���|���ʂ��̗p���Ă��܂��D
	const float GX = 0.048f/2;		//	x�����̕ӂ̒���/2
	const float GY = 0.048f/2;		//	y�����̕ӂ̒���/2
	Matrix3f rotR = Matrix3f::Rot(Rad(-45), 'y');
	Matrix3f rotL = Matrix3f::Rot(Rad(45), 'y');
	Vec3f motorPos[2][8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{
			{rotR*Vec3f(-PX,-PY, PZ), rotR*Vec3f( -GX, 0.0f, 0.0f)},
			{rotR*Vec3f( PX,-PY, PZ), rotR*Vec3f(  GX, 0.0f, 0.0f)},
			{rotR*Vec3f( PX,-PY,-PZ), rotR*Vec3f(0.0f,  -GY, 0.0f)},
			{rotR*Vec3f(-PX,-PY,-PZ), rotR*Vec3f(0.0f,  -GY, 0.0f)},
			{rotR*Vec3f(-PX, PY, PZ), rotR*Vec3f( -GX, 0.0f, 0.0f)},
			{rotR*Vec3f( PX, PY, PZ), rotR*Vec3f(  GX, 0.0f, 0.0f)},
			{rotR*Vec3f( PX, PY,-PZ), rotR*Vec3f(0.0f,   GY, 0.0f)},
			{rotR*Vec3f(-PX, PY,-PZ), rotR*Vec3f(0.0f,   GY, 0.0f)}
		},
		{
			{rotL*Vec3f(-PX, PY,-PZ), rotL*Vec3f(0.0f,   GY, 0.0f)},
			{rotL*Vec3f(-PX, PY, PZ), rotL*Vec3f( -GX, 0.0f, 0.0f)},
			{rotL*Vec3f( PX, PY, PZ), rotL*Vec3f(  GX, 0.0f, 0.0f)},
			{rotL*Vec3f( PX, PY,-PZ), rotL*Vec3f(0.0f,   GY, 0.0f)},
			{rotL*Vec3f(-PX,-PY,-PZ), rotL*Vec3f(0.0f,  -GY, 0.0f)},
			{rotL*Vec3f(-PX,-PY, PZ), rotL*Vec3f( -GX, 0.0f, 0.0f)},
			{rotL*Vec3f( PX,-PY, PZ), rotL*Vec3f(  GX, 0.0f, 0.0f)},
			{rotL*Vec3f( PX,-PY,-PZ), rotL*Vec3f(0.0f,  -GY, 0.0f)},
		}
	};
/*
	//�L�����u���[�V�����ʒu�𒆐S���炸�炷
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/

	if( HISpidarG6::Init(dev, 8, motorPos[bLeft?1:0], 4.0f/35.1f, (float)1.705029838e-5, 1.0f, 20.0f) == false ){
		return false;
	}
	if (bLeft){
		motor[0].lengthPerPulse *= -1;
		motor[3].lengthPerPulse *= -1;
		motor[5].lengthPerPulse *= -1;
		motor[6].lengthPerPulse *= -1;

	}else{
		motor[2].lengthPerPulse *= -1;
		motor[3].lengthPerPulse *= -1;
		motor[4].lengthPerPulse *= -1;
		motor[5].lengthPerPulse *= -1;
	}

	Calib();
	return true;
}

}//	namespace Spr
