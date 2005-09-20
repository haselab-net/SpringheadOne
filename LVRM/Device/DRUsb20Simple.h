// DRUsb20Simple.h: DRUsb20Simple �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRUSB20SIMPLE_H__DF7D202F_F49C_485A_ACBA_27212954662F__INCLUDED_)
#define AFX_DRUSB20SIMPLE_H__DF7D202F_F49C_485A_ACBA_27212954662F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DVCounterBase.h"
#include "DVDaBase.h"
#include "DRRealDevice.h"
#include <WinBasis/WBWin.h>

namespace LVRM {

///	Cyverse��USB2.0(LDR-SPIDAR-AMP)�̃h���C�o
class DRUsb20Simple : public DRRealDevice  
{
	///	���z�f�o�C�X
	class VirtualDeviceDa:public DVDaBase{
	protected:
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceDa(DRUsb20Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	class VirtualDeviceCounter:public DVCounterBase{
	protected:
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceCounter(DRUsb20Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		///	�J�E���^�l�̐ݒ�
		virtual void Count(long c){ realDevice->Count(ch, c); }
		///	�J�E���^�l�̓ǂݏo��
		virtual long Count(){ return realDevice->Count(ch); }
		///	���O
		virtual const char* Name() const{ return name; }
	};
//----------------------------------------------------------------------------
protected:
	char name[100];
	int channel;
	HANDLE hSpidar;

	int sign[8]; //DA�o�͗p�̕���
	long count[8];
	long countOffset[8];
	float voltage[8];

public:
	DRUsb20Simple(int ch);
	virtual ~DRUsb20Simple();

	///	�f�o�C�X�̖��O
	virtual const char* Name() const { return name; }
	///	������
	virtual bool Init();
	///	���z�f�o�C�X�̓o�^
	virtual void Register(DVVirtualDevicePool& vpool);
	
	///	�d���o��
	void Voltage(int ch, float v);
	///	�J�E���^�l�̐ݒ�
	void Count(int ch, long c);
	///	�J�E���^�l�̓ǂݏo��
	long Count(int ch);
	///	��Ԃ̍X�V
	virtual void Update();

	/// ���Z�b�g
	void Reset();

};

} //namespace LVRM

#endif // !defined(AFX_DRUSB20SIMPLE_H__DF7D202F_F49C_485A_ACBA_27212954662F__INCLUDED_)
