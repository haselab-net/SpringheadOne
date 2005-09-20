// HeadStand.cpp: HeadStand �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "HeadStand.h"
#include <Graphics/GRMaterial.h>
#include <sstream>

#include <float.h>
using namespace Spr;

inline bool IsValid(const Vec3f& v){
	return v.square() < Square(900);
}

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

HeadStand::HeadStand()
{
	joHeadStand1 = NULL;
	joHeadStand2 = NULL;
	soHeadStand = NULL;
	checkZero = false;
	checkCount = 0.0;

}

HeadStand::~HeadStand()
{

}


void HeadStand::SaveHSTorque(){
	HeadStandTorque.push_back(joHeadStand1->GetTorque());
}
bool HeadStand::SetHSTorque(float time, float dt){
	int c = (int)(time / dt);
	if (c >= HeadStandTorque.size()) return false;
	joHeadStand2->SetTorque(HeadStandTorque[c]);
	return true;
}
