// IHCSpidar4quad.cpp: IHCSpidar4quad クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "IHCSpidar4quad.hpp"

#include "IHCMotor.hpp"
#include "SH4Matrix.hpp"
#include "IHCDeviceManager.hpp"
#include <string.h>
#include <math.h>
#include "qp_prog01.h"
//#include "iodefine.h"

#ifndef WIN32
//#include "IHCPrinter.h"
#include <cyg/libc/stdio/stdio.h>
//#include <mathf.h>
#include <math.h>
#else
#include <stdio.h>
#endif //WIN32


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
const int IHCSpidar4quad::motorSize = 4;


const int IHCSpidar4quad::SYSTEM_INIT			= 0x02;
const int IHCSpidar4quad::MOTOR_INIT			= 0x04;
const int IHCSpidar4quad::CALIB					= 0x06;
const int IHCSpidar4quad::BEFORE_CALIB			= 0x08;
const int IHCSpidar4quad::AFTER_CALIB			= 0x0a;
const int IHCSpidar4quad::RUN					= 0x0c;
const int IHCSpidar4quad::STOP					= 0x0e;
const int IHCSpidar4quad::EXIT					= 0x10;

IHCSpidar4quad::IHCSpidar4quad()
{
	state = SYSTEM_INIT;
	stiffnessK = 1000;
	int i;
	for(i=0;i<4;i++){
		tension[i] = 1.0f;
	}

	localPos.X() = 0.0f;
	localPos.Y() = 0.0f;
	localPos.Z() = 0.0f;
}

IHCSpidar4quad::~IHCSpidar4quad()
{

}
void IHCSpidar4quad::SetForce(Vec3f& f, float eff, float cont)
{
#ifndef WIN32
	/*	2次計画法による張力計算
		条件式:最小張力 < 張力 < 最大張力
		で張力を求める．	*/
	int i;
	IHCMotor* ptempMotor = *pMotor;
	Vec3f* pphi = phi;

	float* pdata,*ppos_t;
	float pos_t[4],fal;

	for(i=4;i>0;i--){
		pdata = pphi++->data;
		ppos_t = pos_t;

		sub4((ptempMotor++)->pos.data,localPos.data,pos_t);
		fal = sqrt(fipr(pos_t,pos_t));

		*pdata++ = *ppos_t++/fal;
		*pdata++ = *ppos_t++/fal;
		*pdata++ = *ppos_t/fal;
		*pdata = 0.0f;
	}

	/*	目的関数
		  |f-φt|^2 + cont*|t-t'|^2 + eff*t^2
		= (φ^2+eff+cont)*t^2 + (-2*φ*f -2*cont*t')*t + f^2+cont*t'^2
		を最小にする張力tを求める．	*/
	const float val = eff+cont;
	//pphi = phi;
    for( i=0;i<4;i++){
		gMatQ[i][i] = 1.0f+val; //	目的関数の２次係数行列		
	}

	pphi = phi;
	Vec3f* pphi2 = phi+1;
	gMatQ[0][1] = gMatQ[1][0] = fipr(pphi->data   ,pphi2++->data);
	gMatQ[0][2] = gMatQ[2][0] = fipr(pphi->data   ,pphi2++->data);
	gMatQ[0][3] = gMatQ[3][0] = fipr(pphi++->data ,pphi2--->data);
	gMatQ[1][2] = gMatQ[2][1] = fipr(pphi->data   ,pphi2++->data);
	gMatQ[1][3] = gMatQ[3][1] = fipr(pphi++->data ,pphi2->data);
	gMatQ[2][3] = gMatQ[3][2] = fipr(pphi->data   ,pphi2->data);


    for( i=0;i<4;i++){
		gVecX[i] = tension[i];
		gVecC[i] = 2*(fipr(phi[i].data, f.data) + cont*tension[i]);//	目的関数の１次係数ベクトル
		//gVecC[i]=2*fipr(phi[i].data,f.data);//	目的関数の１次係数ベクトル
	}
	//TMU.TSTR.BIT.STR0 = 1;
	// 二次計画法
	quadpr();
	//TMU.TSTR.BIT.STR0 = 0;

	//ptension = tension;

	/*
	pgVecC = gVecX;
	ptempMotor = *pMotor;
    ptempMotor++->Force(*pgVecC++,dumperB);//張力＝gVecX[]
    ptempMotor++->Force(*pgVecC++,dumperB);//張力＝gVecX[]
    ptempMotor++->Force(*pgVecC++,dumperB);//張力＝gVecX[]
    ptempMotor->Force(*pgVecC,dumperB);//張力＝gVecX[]
	*/
    for( i=0;i<4;i++) {
		tension[i] = gVecX[i];
        pMotor[i]->Force(tension[i]);//張力＝gVecX[]
    }
	/*
	count = (0xffffffff - TMU0.TCNT);
	pPrinter->printf("time:%f[μs] (total count:%d)\n\r",count/MHz, count);
	*/
#endif
}

void IHCSpidar4quad::BeforeCalib()
{
	SetMinForce();
	pDevMan->Update();
}
void IHCSpidar4quad::Calib()
{
	for(int i=0;i<4;i++) pMotor[i]->SetOffsetLength();
	localPos.X() = 0.0f;
	localPos.Y() = 0.0f;
	localPos.Z() = 0.0f;
}
void IHCSpidar4quad::AfterCalib()
{
	pDevMan->OpENCCls();
	pDevMan->Update();
}

void IHCSpidar4quad::InitMotor3(){
	printf("Init Motor.\n\r");
	InitMat();
	InitQuadpr(pMotor[0]->getMinF(),pMotor[0]->getMaxF());
}

void IHCSpidar4quad::InitMat()
{
	printf("Init Position Matrix.\n\r");
	localPos.X() = 0.0f;
	localPos.Y() = 0.0f;
	localPos.Z() = 0.0f;

	matPos = 2*Matrix3f(
		pMotor[1]->GetPos()-pMotor[0]->GetPos(),
		pMotor[2]->GetPos()-pMotor[1]->GetPos(),
		pMotor[3]->GetPos()-pMotor[2]->GetPos());
	matPos = matPos.inv();
	matPos.Print("matPos");
	posSqrConst = Vec3f(
		pMotor[1]->GetPos().square()-pMotor[0]->GetPos().square(),
		pMotor[2]->GetPos().square()-pMotor[1]->GetPos().square(),
		pMotor[3]->GetPos().square()-pMotor[2]->GetPos().square());
	posSqrConst.Print("posSqrConst");
}


void IHCSpidar4quad::Update(float freqHz)
{
	//for(int i=0;i<4;i++) len[i] = pMotor[i]->length;
	/*
	Vec3f pos2 = Vec3f(
			Square(len[0]) - Square(len[1]),
			Square(len[1]) - Square(len[2]),
			Square(len[2]) - Square(len[3]));
			*/
	float pos2[4], *ppos = pos2, *pdata = posSqrConst.data;
	float len0 = pMotor[0]->GetLength();
	float len1 = pMotor[1]->GetLength();
	float len2 = pMotor[2]->GetLength();
	float len3 = pMotor[3]->GetLength();

	float lenlen1 = len1*len1;
	float lenlen2 = len2*len2;
//	printf("%f %f %f %f\n\r",len0,len1,len2,len3);

//	printf("pd: %f ", *pdata);
	*ppos++ = len0*len0 - lenlen1   + *pdata++;
//	printf("%f ", *pdata);
	*ppos++ = lenlen1   - lenlen2   + *pdata++;
//	printf("%f ", *pdata);
	*ppos++ = lenlen2   - len3*len3 + *pdata;
	*ppos   = 0.0f;

//	printf("pos2: %f %f %f %f\n\r",pos2[0],pos2[1],pos2[2],pos2[3]);

	Vec3f prevPos = localPos;
#ifndef WIN32
//	ld_ext(matPos.data);
//	ftrv(pos2,localPos.data);
	ftrv(matPos.data, pos2, localPos.data);
#endif //WIN32

//	pos.Print();

	//
	// 物理グリップの速度の計算
	Vec3f prevVelEnc = velEnc;
	//Vec3f velEnc0 = (localPos - prevPos)*freqHz;
	velEnc = (localPos - prevPos);
	//localPos.Print();
	//prevPos.Print();
	/*
	velEnc.X() = iir1filter(0,prevVelEnc.X(),0.9f);
	velEnc.Y() = iir1filter(0,prevVelEnc.Y(),0.9f);
	velEnc.Z() = iir1filter(0,prevVelEnc.Z(),0.9f);
	*/
	//printf("%f %f %f",velEnc.X(),velEnc.Y(),velEnc.Z());
	if (!finitef(prevVelEnc.X()) ){
		prevVelEnc = velEnc;
	}
	velEnc.X() = iir1filter(velEnc.X(),prevVelEnc.X(),0.85f);
	velEnc.Y() = iir1filter(velEnc.Y(),prevVelEnc.Y(),0.85f);
	velEnc.Z() = iir1filter(velEnc.Z(),prevVelEnc.Z(),0.85f);
	//printf("|%f %f %f\n\r",velEnc.X(),velEnc.Y(),velEnc.Z());

	//velEnc *= freqHz;
	//velEnc.Print();
}

float IHCSpidar4quad::iir1filter(float val, float prev, float a){
	return (a * prev + (1.0f - a) * val);
}

void IHCSpidar4quad::SetMinForce()
{
	for(int i=0;i<4;i++){
		pMotor[i]->Force(1.0f);
	}
}

void IHCSpidar4quad::SetMinMinForce()
{
	for(int i=0;i<4;i++){
		pMotor[i]->InitForce();
	}
}

int IHCSpidar4quad::GetState()
{
	return state;
}

void IHCSpidar4quad::SetState(int inState)
{
	this->state = inState;
}
