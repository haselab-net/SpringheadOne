// IHCDebuger.cpp: IHCDebuger �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "IHCDebuger.hpp"
#include <string.h>
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

IHCDebuger::IHCDebuger()
{

}

IHCDebuger::~IHCDebuger()
{

}

// �������[�̓��e���Q�b�c�I�I�I
void IHCDebuger::OpMemGet(const int address,char* data, const int size)
{
	volatile unsigned short *ad;
	ad = (volatile unsigned short*)address;
	memcpy(data,(void*)ad,size);
}

// �������[�ɓ��e���Z�b�c�I�I�I
void IHCDebuger::OpMemSet(const int address,char* data, const int size)
{
	volatile unsigned short *ad;
	ad = (volatile unsigned short*)address;
	memcpy((void*)ad,data,size);
}
