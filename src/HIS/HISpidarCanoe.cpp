#include "HIS.h"
#pragma hdrstop
#include "HISpidarCanoe.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarCanoe, HISpidarG6);

HISpidarCanoe::HISpidarCanoe() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/
void HISpidarCanoe::MakeWireVec(){
	//	�Ƃ�[-��,��]�ɋ���
	if (ori.W() < 0) ori *= -1;
	if (ori.theta() > M_PI*0.7){
//		ori.x *= -1;
/*		Matrix3f mat;
		ori.to_matrix(mat);
		mat.Ex() *= -1;
		mat.Ey() *= -1;
		ori.from_matrix(mat);
*/
		ori = Quaternionf::Rot(Rad(180), 'z') * ori;
		pos.x *= -1;
		DSTR << ori << std::endl;
	}
	
	for(unsigned int i=0; i<motor.size(); ++i){
		wireDirection[i] = motor[i].pos - (ori*motor[i].jointPos + pos);
		calculatedLength[i] = wireDirection[i].norm();
		wireDirection[i] /= calculatedLength[i];
		wireMoment[i] = (ori * motor[i].jointPos) ^ wireDirection[i];
	}
}
void HISpidarCanoe::Update(float dt){

	HISpidarG6::Update(dt);
}
bool HISpidarCanoe::Init(DVDeviceManager& dev){
	//	���̃O���b�v�ւ̎��t���ʒu
	const float GX = 1.27f/2.0f;		//	x�����̕ӂ̒���/2
	const float GY = 0.0f;				//	y�����̕ӂ̒���/2
	const float GZ = 0.40f/2.0f;		//	z�����̕ӂ̒���/2

#if 0	//	�~�[�e�B���O���[��
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	const float PX = 2.76f/2;		//	x�����̕ӂ̒���/2
	const float PY = 2.50f/2;		//	y�����̕ӂ̒���/2
	const float PZ = 2.085f/2;		//	z�����̕ӂ̒���/2
	Vec3f motorPos[8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f(-PX, PY, PZ), Vec3f( -GX, GY,  GZ)},//����O
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, GY, -GZ)},//�E�㉜
		{Vec3f(-PX,-PY,-PZ), Vec3f( -GX, GY, -GZ)},//������
		{Vec3f( PX,-PY, PZ), Vec3f(  GX, GY,  GZ)},//�E���O
		{Vec3f( PX, PY, PZ), Vec3f(  GX, GY,  GZ)},//�E��O
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, GY, -GZ)},//���㉜
		{Vec3f( PX,-PY,-PZ), Vec3f(  GX, GY, -GZ)},//�E����
		{Vec3f(-PX,-PY, PZ), Vec3f( -GX, GY,  GZ)} //�����O
	};
#endif
#if 1	//	�H�쎺
	const float PX = 0.061f/2;		//	x�����̕ӂ̒���/2
	const float PY = 2.35f/2;		//	y�����̕ӂ̒���/2
	const float PZ = 2.10f/2;		//	z�����̕ӂ̒���/2
	Vec3f motorPos[8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f( PX,-PY,-PZ), Vec3f(  GX, -GZ,  0)},
		{Vec3f(-PX,-PY,-PZ), Vec3f( -GX, -GZ,  0)},
		{Vec3f(-PX,-PY, PZ), Vec3f( -GX, -GZ,  0)},
		{Vec3f( PX,-PY, PZ), Vec3f(  GX, -GZ,  0)},
		{Vec3f( PX, PY,-PZ), Vec3f(  GX,  GZ,   0)},
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX,  GZ,   0)},
		{Vec3f(-PX, PY, PZ), Vec3f( -GX,  GZ,   0)},
		{Vec3f( PX, PY, PZ), Vec3f(  GX,  GZ,   0)},
	};
#endif
#if 0	//	Etech
	const float In = 0.0254f;
	const float PX = 2.5	*In/2;		//	x�����̕ӂ̒���/2
	const float PY = 101	*In/2;		//	y�����̕ӂ̒���/2
	const float PZ = 109	*In/2;		//	z�����̕ӂ̒���/2
	const float PZ2 = 85*In	-PZ;		//	
	Vec3f motorPos[8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f(-PX,-PY, PZ), Vec3f( -GX, GY,  GZ)},
		{Vec3f( PX,-PY, PZ), Vec3f(  GX, GY,  GZ)},
		{Vec3f( PX,-PY,-PZ2), Vec3f(  GX, GY, -GZ)},
		{Vec3f(-PX,-PY,-PZ2), Vec3f( -GX, GY, -GZ)},
		{Vec3f(-PX, PY, PZ), Vec3f( -GX, GY,  GZ)},
		{Vec3f( PX, PY, PZ), Vec3f(  GX, GY,  GZ)},
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, GY, -GZ)},
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, GY, -GZ)},
	};
#endif
	/*
	Vec3f motorPos[8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f(-PX,-PY, PZ), Vec3f(0.0f,  -GY, 0.0f)},
		{Vec3f(-PX,-PY,-PZ), Vec3f( -GX, 0.0f, 0.0f)},
		{Vec3f( PX,-PY,-PZ), Vec3f(  GX, 0.0f, 0.0f)},
		{Vec3f( PX,-PY, PZ), Vec3f(0.0f,  -GY, 0.0f)},
		{Vec3f(-PX, PY, PZ), Vec3f(0.0f,   GY, 0.0f)},
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, 0.0f, 0.0f)},
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, 0.0f, 0.0f)},
		{Vec3f( PX, PY, PZ), Vec3f(0.0f,   GY, 0.0f)}
	};
	*/
/*
	//�L�����u���[�V�����ʒu�𒆐S���炸�炷
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/
	double lpp = 3.0824008138351983723296032553408e-5 * 500 /1024;
	if( HISpidarG6::Init(dev, 8, motorPos, 0.3f, (float)lpp, 0.8f, 8.0f) == false ){
		return false;
	}
	motor[1].lengthPerPulse *= -1;
	motor[3].lengthPerPulse *= -1;
	motor[4].lengthPerPulse *= -1;
	motor[6].lengthPerPulse *= -1;

	Calib();
	return true;
}

}//	namespace Spr
