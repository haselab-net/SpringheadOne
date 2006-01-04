// HISpidarFinger.cpp: HISpidarFinger クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////


#include "HISpidarHand.h"
#include "HISpidarFinger.h"
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr {

HISpidarFinger::HISpidarFinger()
{

}

HISpidarFinger::~HISpidarFinger()
{

}

void HISpidarFinger::InitInstance(HISpidarHand* pHand,HISpidarMotor* pMotor1,HISpidarMotor* pMotor2 = 0)
{
	this->m_pHand = pHand;
	this->m_pMotor1 = pMotor1;
	BeforeCalib();
}

//最低張力の設定(0.1N)
void HISpidarFinger::SetMinForce()
{
	m_pMotor1->SetForce(0.1f);
}

//指先を90度曲げてキャリブレーションもしくは、指先を１０ｃｍ伸ばして
bool HISpidarFinger::FingerCalib()
{
	FingerEnc1[0]=m_pMotor1->counter->Count();
	return true;
}

//指先を伸ばしてキャリブレーション 
bool HISpidarFinger::FingerCalib1()
{
	FingerEnc1[1]=m_pMotor1->counter->Count();

	//今の指の位置
	FingerPos1.X()=m_pHand->GetPos().X()+InitFingerPos1.X();
	FingerPos1.Y()=m_pHand->GetPos().Y()+InitFingerPos1.Y();
	FingerPos1.Z()=m_pHand->GetPos().Z()+InitFingerPos1.Z();

	//1pulseあたりの角度を決定
	RadPerPlus1=(3.141592/2)/(FingerEnc1[0]-FingerEnc1[1]);
	//1pulseあたりの長さを決定
	LengthPerPlus1 = 0.05/(FingerEnc1[1]-FingerEnc1[0]);

	return true;
}

void HISpidarFinger::Calc_FingerPos()
{	
	///現在のエンコーダ値から指の曲がり角度を設定
	FingerRad1 = m_pMotor1->counter->Count()*RadPerPlus1;
	///現在のエンコーダ値から糸の長さを設定
	FingerLength1 = m_pMotor1->counter->Count()*LengthPerPlus1;

	///指の位置の更新
	//X座標
	FingerPos1.X()=m_pHand->GetPos().X()+InitFingerPos1.X();

	//Y座標
	FingerPos1.Y()=m_pHand->GetPos().Y()+InitFingerPos1.Y()-50*sin(FingerRad1);

	//Z座標
	FingerPos1.Z()=m_pHand->GetPos().Z()+InitFingerPos1.Z()-50*cos(FingerRad1);

 }

void HISpidarFinger::Update()
{
	Calc_FingerPos();
}


}//namespace Spr 