// DRUsb20Sh4.h: DRUsb20Sh4 �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef SPR_DRUsb20Sh4_H
#define SPR_DRUsb20Sh4_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRUsb20Simple.h"
#include "DVAdBase.h"

namespace Spr {

///	USB2.0 SH �A���v �̃h���C�o
class SPR_DLL DRUsb20Sh4 : public DRUsb20Simple{
//----------------------------------------------------------------------------
protected:
	float adVoltPerDigit;
public:
	///	���z�f�o�C�X(DA)
	class VirtualDeviceAd:public DVAdBase{
	protected:
		int ch;
		DRUsb20Sh4* realDevice;
		char name[100];
	public:
		VirtualDeviceAd(DRUsb20Sh4* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual float Voltage(){ return realDevice->AdVoltage(ch); }
		virtual int Digit(){ return realDevice->AdDigit(ch); }
		virtual const char* Name() const{ return name; }
	};

	///	�R���X�g���N�^	ch�͔w�ʂ̃X�C�b�`�ɂȂ�\��
	DRUsb20Sh4(int ch);
	/// ���Z�b�g
	virtual void Reset();
	///	virtual device �̓o�^
	virtual void Register(DVVirtualDevicePool& vpool);	

	float AdVoltage(int ch){ return adIn[ch] * adVoltPerDigit; }
	int AdDigit(int ch){ return adIn[ch]; }
	
	///	��Ԃ̍X�V
	virtual void Update();

	///	512byte�̑��M
	virtual void UsbSend(unsigned char* outBuffer);
	///	512byte�̎�M
	virtual void UsbRecv(unsigned char* inBuffer);
protected:
	virtual void UsbUpdate();
	virtual bool FindDevice(int ch);

};

} //namespace Spr

#endif // SPR_DRUsb20Sh4_H
