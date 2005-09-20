// IHCMotor.cpp: IHCMotor クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "IHCMotor.hpp"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
//#include "ihcprinter.h"
#include <cyg/libc/stdio/stdio.h>

IHCMotor::IHCMotor()
{
	maxForce = 20;
	minForce = 1;
	ampPerNewton = 0.365296803653f;
	lengthPerPulse = 2.924062107079e-5f;
	voltPerHex	= 28.0f / 4096.0f;
	//hexPerAmp = 426.666666666666666666666666f;
	//hexPerAmp = 426.666666666666666666666666f;
	//hexPerAmp = 311.71322383407265315909363385685f;

	pulleyRadius = 8;
	enc_phs = 0x1;

	amp = 0;
//	rps = 0;
}

void IHCMotor::Force(float f)
{
	if(minForce > f){
		f = minForce;
	}
	else if(maxForce < f){
		f = maxForce;
	}
	amp = f * ampPerNewton ;
}

void IHCMotor::InitForce(float a /*= 0.5f*/)
{
	amp = a * minForce * ampPerNewton;
}


void IHCMotor::printInfo(){
	printf("pos(%10f,%10f,%10f) phs[%c(%d)]\n\r",
		pos.X(),pos.Y(),pos.Z(),
		((enc_phs==1) ? 'B' : 'A'),enc_phs);

	printf("\t apn:%10f, hpa:%10f, lpp:%10f, vph:%10f\n\r",
		ampPerNewton,
		hexPerAmp,
		lengthPerPulse,
		voltPerHex);

	printf("\t minF:%10f, maxF:%10f\n\r",minForce,maxForce);

	printf("\ttorqueConst:%10f, terminalResist:%10f, terminalInduct:%10f, speedConst:%10f\n\r"
		,torqueConst, terminalResist, terminalInduct, speedConst);

	printf("\tPullryRadius:%10f\n\r",pulleyRadius);
}


void IHCMotor::SetMotorInfo(float minF,float maxF,float torqueConst,float terminalResist, float terminalInduct,float speedConst,float pulleyRadius)
{
	this->minForce = minF;
	this->maxForce = maxF;
	this->torqueConst = torqueConst;
	this->terminalResist = terminalResist;
	this->terminalInduct = terminalInduct;
	this->speedConst = speedConst;
	this->pulleyRadius = pulleyRadius;
}

void IHCMotor::SetAmpInfo(float apn,float hpa, float lpp, float vph)
{
	this->ampPerNewton = apn;
	this->hexPerAmp = hpa;
	this->lengthPerPulse = lpp;
	this->voltPerHex = vph;
}

void IHCMotor::calcLength(short hexEnc)
{
	this->length = - hexEnc * lengthPerPulse + offsetLength;
}
void IHCMotor::calcLength(short hexEnc, short hexAd, float freqHz)
{
	float prevL = this->length;
	float prevV = this->vel_length;

	this->volt = 24.0f - hexAd * voltPerHex;
	this->length = - hexEnc * lengthPerPulse + offsetLength;

	this->vel_length = (this->length - prevL) * freqHz;			//速度
	this->acc_length = (this->vel_length - prevV) * freqHz;		//加速度
}

void IHCMotor::calcVolt(short hexAd)
{
	this->volt = 24.0f - hexAd * voltPerHex;
}

unsigned short IHCMotor::calcHexDA()
{
	return (unsigned short)(int)(amp * hexPerAmp);
}

void IHCMotor::SetEncPhs(unsigned char phs)
{
	this->enc_phs = phs;
}

void IHCMotor::SetOffsetLength()
{
	printf ("pos: %f %f %f %f\n", pos.X(), pos.Y(), pos.Z(), pos.norm());
	SetOffsetLength(pos.norm());
}
void IHCMotor::SetOffsetLength(float offset)
{
	this->offsetLength = offset;
}

float IHCMotor::GetLength()
{
	return this->length;
}

void IHCMotor::SetPos(float x, float y, float z)
{
	this->pos.X() = x;
	this->pos.Y() = y;
	this->pos.Z() = z;
}

Vec3f IHCMotor::GetPos()
{
	return this->pos;
}

