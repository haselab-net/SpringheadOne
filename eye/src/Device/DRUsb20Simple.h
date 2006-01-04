// DRUsb20Simple.h: DRUsb20Simple �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef SPR_DRUSB20SIMPLE_H
#define SPR_DRUSB20SIMPLE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DVCounterBase.h"
#include "DVDaBase.h"
#include "DVPioBase.h"
#include "DRRealDevice.h"
#include <WinBasis/WBWin.h>

namespace Spr {

///	Cyverse��USB2.0(LDR-SPIDAR-AMP)�̃h���C�o
class SPR_DLL DRUsb20Simple : public DRRealDevice  
{
	///	���z�f�o�C�X(DA)
	class VirtualDeviceDa:public DVDaBase{
	protected:
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceDa(DRUsb20Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual void Digit(int v){ realDevice->Digit(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	///	���z�f�o�C�X(Counter)
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
	///	���z�f�o�C�X(Pio)
	class VirtualDevicePio:public DVPioBase{
	protected:
		int ch;
		DRUsb20Simple* realDevice;
		char name[100];
	public:
		VirtualDevicePio(DRUsb20Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Set(int l){ realDevice->Pio(ch, l!=0); }
		virtual int Get(){ return realDevice->Pio(ch) ? 1 : 0; }
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
	int daOut[8];
	int adIn[8];
	int pioLevel[16];

public:
	///	�R���X�g���N�^	ch�͔w�ʂ̃X�C�b�`�ɂȂ�\��
	DRUsb20Simple(int ch);
	virtual ~DRUsb20Simple();

	///	�f�o�C�X�̖��O
	virtual const char* Name() const { return name; }
	///	������
	virtual bool Init();
	///	������(�`�����l��, PIDVID���C�ɂ����ɏ�����)
	virtual bool InitAny();
	///	���z�f�o�C�X�̓o�^
	virtual void Register(DVVirtualDevicePool& vpool);
	
	///	�d���o��
	void Voltage(int ch, float v);
	///	�d���o��(���l�w��)
	void Digit(int ch, int v);
	///	�J�E���^�l�̐ݒ�
	void Count(int ch, long c);
	///	�J�E���^�l�̓ǂݏo��
	long Count(int ch);
	///	��Ԃ̍X�V
	virtual void Update();
	
	///	PIO�|�[�g�̐ݒ�
	void Pio(int ch, bool level);
	///	PIO�|�[�g�̓ǂݏo��
	bool Pio(int ch);
	///	�X�C�b�`�i�|�[�g�̏��5-8�r�b�g�j�̓ǂݏo��
	int RotarySwitch();

	/// ���Z�b�g
	virtual void Reset();

	///	USB�̃t�@�C���n���h��
	HANDLE GetHandle(){ return hSpidar; };
protected:
	virtual void UsbReset();
	virtual void UsbCounterGet();
	virtual void UsbCounterClear();
	virtual void UsbDaSet();
	virtual DWORD UsbVidPid(HANDLE h);
	virtual bool FindDevice(int ch);
	virtual HANDLE UsbOpen(int id);
	virtual bool UsbClose(HANDLE& h);
};

} //namespace Spr

#endif // SPR_DRUSB20SIMPLE_H
