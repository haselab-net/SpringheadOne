#if !defined(AFX_DRUSBH8_H__618658E7_7169_4BF7_A25E_562BC6BDEE17__INCLUDED_)
#define AFX_DRUSBH8_H__618658E7_7169_4BF7_A25E_562BC6BDEE17__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DVCounterBase.h"
#include "DVDaBase.h"
#include "DRRealDevice.h"
#include <WinBasis/WBWin.h>

namespace LVRM {

///	Cyverse��USB�W�����[�^�h���C�oH8�ł̃h���C�o.
class DRUsbH8Simple:public DRRealDevice
{
public:
	///	���z�f�o�C�X
	class VirtualDeviceDa:public DVDaBase{
	protected:
		int ch;
		DRUsbH8Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceDa(DRUsbH8Simple* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	class VirtualDeviceCounter:public DVCounterBase{
	protected:
		int ch;
		DRUsbH8Simple* realDevice;
		char name[100];
	public:
		VirtualDeviceCounter(DRUsbH8Simple* r, int c);
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
	
	int sign[8];
	long count[8];
	long countOffset[8];
	float voltage[8];
public:
	DRUsbH8Simple(int ch);
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
};

}	//	namespace LVRM;

#endif // !defined(AFX_DRUSBH8_H__618658E7_7169_4BF7_A25E_562BC6BDEE17__INCLUDED_)
