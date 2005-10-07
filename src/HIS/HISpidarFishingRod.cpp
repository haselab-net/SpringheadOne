#include "HIS.h"
#ifdef _WIN32
 #pragma hdrstop
#endif
#include "HISpidarFishingRod.h"

namespace Spr {;
HIOBJECTIMP(HISpidarFishingRod, HIForceDevice3D);
//----------------------------------------------------------------------------
HISpidarFishingRod::HISpidarFishingRod(){
	SetWeight();
}
bool HISpidarFishingRod::Init(DVDeviceManager& dev)//, int nMotor, const Vec3f(* motorPos)[2], float vpn, float lpp, float minF, float maxF){
{
	int nMotor = 8;
	float vpn = 0.3f;
	float lpp = -(float)2.924062107079e-5;
	//float lpp = -(float)1.03e-5;
	float minF = 0.3f;
	//float minF = 1.4f;
	float maxF = 12.0f;

//	���̃O���b�v�ւ̎��t���ʒu
	const float GX = 0.0f;
	const float GY = 0.0f;
	const float GZ = 0.0f;

	const float PX1 = 0.79f;		//	x�����̕ӂ̒���/2
	const float PX2 = 0.86f;		//	x�����̕ӂ̒���/2
	const float PX3 = 0.0f;
	const float PY = 0.0f;		//	y�����̕ӂ̒���/2
	const float PZ1 = 0.4025f;		//	z�����̕ӂ̒���/2
	const float PZ2 = 0.0f;

	/*
	Vec3f motorPos[8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f(-PX1, PY,-PZ1), Vec3f(-GX, GY,-GZ)},
		{Vec3f(-PX2, PY, PZ2), Vec3f(-GX, GY, GZ)},
		{Vec3f(-PX1, PY, PZ1), Vec3f(-GX, GY, GZ)},
		{Vec3f( PX3, PY, PZ1), Vec3f( GX, GY, GZ)},
		{Vec3f( PX1, PY, PZ1), Vec3f( GX, GY, GZ)},
		{Vec3f( PX2, PY, PZ2), Vec3f( GX, GY, GZ)},
		{Vec3f( PX1, PY,-PZ1), Vec3f( GX, GY,-GZ)},
		{Vec3f( PX3, PY,-PZ1), Vec3f( GX, GY,-GZ)},
	};
*/
	const float YOFF=-0.2f;//20cm�オ���_�Ƃ���
	Vec3f	motorPos[1][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f(-PX1, PY+YOFF,-PZ1), Vec3f(-GX, GY,-GZ)},// 1
//		{Vec3f(-PX2, PY+YOFF, PZ2), Vec3f(-GX, GY, GZ)},// 2
//		{Vec3f(-PX1, PY+YOFF, PZ1), Vec3f(-GX, GY, GZ)},// 3
//		{Vec3f( PX3, 0.58+YOFF, PZ1), Vec3f( GX, GY, GZ)},// 4
//		{Vec3f( PX1, PY+YOFF, PZ1), Vec3f( GX, GY, GZ)},// 5
//		{Vec3f( PX2, PY+YOFF, PZ2), Vec3f( GX, GY, GZ)},// 6
//		{Vec3f( PX1, PY+YOFF,-PZ1), Vec3f( GX, GY,-GZ)},// 7
//		{Vec3f( PX3, PY+YOFF,-PZ1), Vec3f( GX, GY,-GZ)},// 8
		
		
		
		
		
	};

/*
	//�L�����u���[�V�����ʒu�𒆐S���炸�炷
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/

//	if( HISpidarG6::Init(dev, 8, motorPos, 0.3f, (float)2.924062107079e-5, 1.0f, 15.0f) == false ){
//		return false;
//	}


	nRepeat = 2;
	sigma = 0.001f; //sigma=sigma*sigma
	motor.resize(nMotor);
	VVector<float> minForce, maxForce;
	minForce.resize(nMotor);
	maxForce.resize(nMotor);
	for(int i=0; i<nMotor; ++i){
		motor[i].minForce = minForce[i] = minF;
		motor[i].maxForce = maxForce[i] = maxF;
		motor[i].pos = motorPos[i][0];
		motor[i].jointPos = motorPos[i][1];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
	}

	HISpidarCalc3Dof::Init(3, minForce, maxForce);

	motor[1].lengthPerPulse *= -1;
//	motor[3].lengthPerPulse *= -1;
//	motor[5].lengthPerPulse *= -1;
//	motor[7].lengthPerPulse *= -1;


	//	�h���C�o�̎擾
	unsigned int i;
	for(i=0; i<motor.size(); ++i){
		if (!dev.VPool().Rent(motor[i].da)) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<motor.size()) return false;
	for(i=0; i<motor.size(); ++i){
		if (!dev.VPool().Rent(motor[i].counter)) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	button.resize(8);
	for(i=0; i<button.size(); ++i){
		if (!dev.VPool().Rent(button[i])) break;
		AddRealDeviceDependency(button[i]->RealDevice());
	}

	SetMinForce();
	BeforeCalib();
	Calib();
	AfterCalib();
	bGood = true;

	return true;
}
bool HISpidarFishingRod::Calib(){
	//	�|�C���^�����_(���S)�ɒu���āA�L�����u���[�V�������s��
	for(unsigned i=0; i<motor.size(); i++) motor[i].SetLength( (motor[i].pos - motor[i].jointPos).norm() );
	lengthDiffAve.clear();
	for(int i=0; i<4; ++i) HISpidarCalc3Dof::Update();	//	�p�����X�V
	return true;
}
void HISpidarFishingRod::Update(float dt){
	HIForceDevice3D::Update(dt);
	HISpidarCalc3Dof::Update();
	for(unsigned int i=0; i<motor.size(); ++i){
		motor[i].SetForce(Tension()[i]);
	}
}
void HISpidarFishingRod::SetMinForce(){
	for(unsigned int i=0; i<motor.size(); ++i){
		motor[i].SetForce(motor[i].minForce);
	}
	Update(0.001f);
}
void HISpidarFishingRod::MakeWireVec(){
	for(unsigned int i=0; i<motor.size(); ++i){
		wireDirection[i] = motor[i].pos -  pos;
		calculatedLength[i] = wireDirection[i].norm();
		wireDirection[i] /= calculatedLength[i];
//		wireMoment[i] = (ori * motor[i].jointPos) ^ wireDirection[i];
	}
}
void HISpidarFishingRod::UpdatePos(){
}
void HISpidarFishingRod::MeasureWire(){
	for(unsigned int i=0; i<motor.size(); ++i){
		measuredLength[i] = motor[i].GetLength();
	}	
}

double HISpidarFishingRod::GetLength(int i)
{
	return motor[i].GetLength();	 
}



}//	namespace Spr


/*
#include "HIS.h"
#pragma hdrstop
#include "HISpidarFishingRod.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarFishingRod, HISpidarG6);

HISpidarFishingRod::HISpidarFishingRod() : HISpidarG6() {
}

bool HISpidarFishingRod::Init(DVDeviceManager& dev){
	//	���̃O���b�v�ւ̎��t���ʒu
	const float GX = 0.0f;
	const float GY = 0.0f;
	const float GZ = 0.0f;

	const float PX1 = 0.79f;		//	x�����̕ӂ̒���/2
	const float PX2 = 0.86f;		//	x�����̕ӂ̒���/2
	const float PX3 = 0.0f;
	const float PY = 0.0f;		//	y�����̕ӂ̒���/2
	const float PZ1 = 0.4025f;		//	z�����̕ӂ̒���/2
	const float PZ2 = 0.0f;

	/*
	Vec3f motorPos[8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f(-PX1, PY,-PZ1), Vec3f(-GX, GY,-GZ)},
		{Vec3f(-PX2, PY, PZ2), Vec3f(-GX, GY, GZ)},
		{Vec3f(-PX1, PY, PZ1), Vec3f(-GX, GY, GZ)},
		{Vec3f( PX3, PY, PZ1), Vec3f( GX, GY, GZ)},
		{Vec3f( PX1, PY, PZ1), Vec3f( GX, GY, GZ)},
		{Vec3f( PX2, PY, PZ2), Vec3f( GX, GY, GZ)},
		{Vec3f( PX1, PY,-PZ1), Vec3f( GX, GY,-GZ)},
		{Vec3f( PX3, PY,-PZ1), Vec3f( GX, GY,-GZ)},
	};
*/
/*		Vec3f motorPos[8][2] = {		//	���[�^�̎��t���ʒu(���S�����_�Ƃ���DirectX���W�n�i�E��X,�オY,����Z�j)
		{Vec3f( PX1, PY,-PZ1), Vec3f( GX, GY,-GZ)},// 7
		{Vec3f( PX2, 0.58, PZ2), Vec3f( GX, GY, GZ)},// 6
		{Vec3f( PX1, PY, PZ1), Vec3f( GX, GY, GZ)},// 5
		{Vec3f( PX3, 0.58, PZ1), Vec3f( GX, GY, GZ)},// 4
		{Vec3f(-PX1, PY, PZ1), Vec3f(-GX, GY, GZ)},// 3
		{Vec3f(-PX2, 0.58, PZ2), Vec3f(-GX, GY, GZ)},// 2
		{Vec3f(-PX1, PY,-PZ1), Vec3f(-GX, GY,-GZ)},// 1
		{Vec3f( PX3, 0.40,-PZ1), Vec3f( GX, GY,-GZ)},// 8
	};
/*
	//�L�����u���[�V�����ʒu�𒆐S���炸�炷
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/
/*
	if( HISpidarG6::Init(dev, 8, motorPos, 0.3f, (float)2.924062107079e-5, 1.0f, 15.0f) == false ){
		return false;
	}

	motor[1].lengthPerPulse *= -1;
	motor[3].lengthPerPulse *= -1;
	motor[5].lengthPerPulse *= -1;
	motor[7].lengthPerPulse *= -1;

	Calib()	
	return true;
}

double HISpidarFishingRod::GetLength(int i)
{
	return motor[i].GetLength();	 
}

}//	namespace Spr
*/