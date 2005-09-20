// IHCDebuger.cpp: IHCDebuger クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "IHCDebuger.hpp"
#include <string.h>
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

IHCDebuger::IHCDebuger()
{

}

IHCDebuger::~IHCDebuger()
{

}

// メモリーの内容をゲッツ！！！
void IHCDebuger::OpMemGet(const int address,char* data, const int size)
{
	volatile unsigned short *ad;
	ad = (volatile unsigned short*)address;
	memcpy(data,(void*)ad,size);
}

// メモリーに内容をセッツ！！！
void IHCDebuger::OpMemSet(const int address,char* data, const int size)
{
	volatile unsigned short *ad;
	ad = (volatile unsigned short*)address;
	memcpy((void*)ad,data,size);
}
