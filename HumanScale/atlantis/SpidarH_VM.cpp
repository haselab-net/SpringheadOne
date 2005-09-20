#include <His/HIS.h>
#pragma hdrstop
#include "SpidarH_VM.h"

namespace LVRM {;
//----------------------------------------------------------------------------
SpidarH_VM::SpidarH_VM()  
{
}

bool SpidarH_VM::Init(){


	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();

	// VM��Big-SM_PIDAR�p�@���[�^�̎��t�����W�i�^�[���e�[�u���̒��S���珰��110cm�����_�j	
	/*Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-1.75f, 2.00f, 1.12f),	Vec3f( 1.65f, 2.17f,-1.34f), Vec3f(-1.65f,-0.9f,-1.44f), Vec3f( 1.65f,-0.99f, 0.50f)},
		{Vec3f( 1.58f, 2.00f, 1.12f),	Vec3f(-1.65f, 2.18f,-1.34f), Vec3f( 1.65f,-0.98f,-1.44f), Vec3f(-1.65f,-0.98f, 0.50f)}
	};*/
	// 2003.10 NEW SETTING
	//���[�^�̎��t���ʒu(���S�����_�Ƃ���)

	Vec3f motorPos[][4] = {
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};


	if( h_spidar[0].Init(devMan, motorPos[0], MAXON, -LPP, 0.5f, 20.0f) == true )
        h_spidar[0].Calib();
	else return false;
	
	if( h_spidar[1].Init(devMan, motorPos[1], MAXON, LPP, 0.5f, 20.0f) == true )
        h_spidar[1].Calib();
	else return false;

	bMinForce = true;
	
	return true;
}

void SpidarH_VM::Update(int idx)
{
  h_spidar[idx].Update();
}

void SpidarH_VM::SetMinForce(int idx)
{
  h_spidar[idx].SetMinForce();
}

Vec3f SpidarH_VM::GetForce(int idx)
{
  Vec3f f;
  f = h_spidar[idx].GetForce();
  return f;
}

Vec3f SpidarH_VM::GetPos(int idx)
{
  Vec3f pp;
  pp = h_spidar[idx].GetPos();
  return pp;
}
/*
/////////////////////////////
void TGLView::init_spidar()
{
	//devMan.RPool().Register(new DRTokin3D);
    //devMan.RPool().Register(new DRUsb20Simple(0));
	//devMan.Init();
	//std::cout << devMan;
   
		//�H�쎺��SPIDAR
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};

	// 2003.10 NEW SETTING
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};

	bMinForce = true;
	holdHand  = false;
	ballCount = 0;
	for(int i=0; i<2; i++)
	{
	   h_spidar[i].Init(devMan, motorPos[i], MAXON, LPP, 0.5f, 20.0f); 
	   h_spidar[i].Calib();
	}

}*/


void SpidarH_VM::reset_spidar()
{
	for(int i = 0; i < 2; i++) h_spidar[i].Calib();
	
}

//////////////////// SPIDAR ����m�F ////////////////////
void SpidarH_VM::spidarCheck(){
		// SPIDAR �̍��W�\��
		for(int j = 0; j < 2; j++){
			printf("%d: (x,y,z) = (%f, %f, %f)\n", j, h_spidar[j].GetPos().X(), h_spidar[j].GetPos().Y(), h_spidar[j].GetPos().Z());
		}
		printf("\n");

		// ���[�^�[�̎��̒����\��
		/*for(int j = 0; j < 2; j++){
			for(int i = 0; i < 4; i++){
				std::cout << i + 4 * j << ": " << h_spidar[j].Motor()[i].Length()  << " ";
			}
			printf("\n");
		}
		printf("\n");*/
}

Vec3f SpidarH_VM::convertEuclidToPole(Vec3f euclid){
	Vec3f pole;	// X�F���a�AY�F�ƁAZ�F�� �Ƃ���B
	pole.X() = sqrt(pow(euclid.X(), 2) + pow(euclid.Y(), 2) + pow(euclid.Z(), 2));
	pole.Y() = atan2(euclid.Z(), euclid.X());
	pole.Z() = atan2(euclid.Y(), sqrt(pow(euclid.X(), 2) + pow(euclid.Z(), 2)));
	return pole;
}

Vec3f SpidarH_VM::convertPoleToEuclid(Vec3f pole){
	Vec3f euclid;	// X�F���a�AY�F�ƁAZ�F�� �Ƃ���B
	euclid.X() = pole.X() * cos(pole.Y()) * cos(pole.Z());
	euclid.Y() = pole.X() * sin(pole.Z());
	euclid.Z() = pole.X() * sin(pole.Y()) * cos(pole.Z());
	return euclid;
}

}//	namespace LVRM