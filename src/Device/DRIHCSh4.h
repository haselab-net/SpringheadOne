#ifndef SPR_DRIHCSh4_H
#define SPR_DRIHCSh4_H

#ifdef _WIN32
 #error This program runs on SH4
#endif


#include <Base/Env.h>
#include "DRRealDevice.h"
#include "DVDaBase.h"
#include "DVAdBase.h"
#include "DVPioBase.h"
#include "DVCounterBase.h"

namespace Spr {

/// DRIHCSh4.cpp: SH4��œ����C�f�o�C�X�A�N�Z�X�N���X
class SPR_DLL DRIHCSh4 : public DRRealDevice{
public:
	///	���z�f�o�C�X(DA)
	class VirtualDeviceDa:public DVDaBase{
	protected:
		DRIHCSh4* realDevice;
		int ch;
		char name[100];
	public:
		VirtualDeviceDa(DRIHCSh4* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->DaVoltage(ch, v); }
		virtual void Digit(int v){ realDevice->DaDigit(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	///	���z�f�o�C�X(Counter)
	class VirtualDeviceCounter:public DVCounterBase{
	protected:
		DRIHCSh4* realDevice;
		int ch;
		char name[100];
	public:
		VirtualDeviceCounter(DRIHCSh4* r, int c);
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
		DRIHCSh4* realDevice;
		int ch;
		char name[100];
	public:
		VirtualDevicePio(DRIHCSh4* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Set(int l){ realDevice->Pio(ch, l!=0); }
		virtual int Get(){ return realDevice->Pio(ch) ? 1 : 0; }
		virtual const char* Name() const{ return name; }
	};
	///	���z�f�o�C�X(AD)
	class VirtualDeviceAd:public DVAdBase{
	protected:
		DRIHCSh4* realDevice;
		int ch;
		char name[100];
	public:
		VirtualDeviceAd(DRIHCSh4* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual float Voltage(){ return realDevice->AdVoltage(ch); }
		virtual int Digit(){ return realDevice->AdDigit(ch); }
		virtual const char* Name() const{ return name; }
	};

protected:
	char name[100];
	float adVoltPerDigit;

	long count[8];
	long countOffset[8];
	int daOut[8];
	int adIn[8];
	int pioLevel[16];

public:
	DRIHCSh4();
	virtual ~DRIHCSh4();
	virtual const char* Name() const { return "IHCSh4"; }
	virtual bool Init();
	///	virtual device �̓o�^
	virtual void Register(DVVirtualDevicePool& vpool);

	///	�d���o��
	void DaVoltage(int ch, float v){
		assert(0 <= ch && ch < 8);
		const float DigitPerVolt[]={	//	DA�w�ߒl/�o�͓d�� �A���v���ƂɈقȂ�̂Ōv���l���A���v�ɏ����Ă����C�ǂݏo���K�v������D
			415.0f,
			415.0f,
			415.0f,
			415.0f,
			415.0f,
			415.0f,
			415.0f,
			415.0f,
		};
		daOut[ch] = (int)(v * DigitPerVolt[ch]);
	}
	///	�d���o��(���l�w��)
	void DaDigit(int ch, int v){
		daOut[ch] = v;
	}
	///	�J�E���^�l�̐ݒ�
	void Count(int ch, long c){
		countOffset[ch] = - (count[ch] - c); 
	}
	///	�J�E���^�l�̓ǂݏo��
	long Count(int ch){
		return count[ch] + countOffset[ch];
	}
	///	PIO�|�[�g�̐ݒ�
	void Pio(int ch, bool level){
		pioLevel[ch] = level ? 1 : 0;
	}
	///	PIO�|�[�g�̓ǂݏo��
	bool Pio(int ch){
		return pioLevel[ch] == 1;
	}
	///	�X�C�b�`�i�|�[�g�̏��5-8�r�b�g�j�̓ǂݏo��
	int RotarySwitch(){
		int sw=0;
		for(int i=0; i<4; ++i){
			sw |= pioLevel[i+4] << i;
		}
		return sw;
	}
	///	AD�̓d��
	float AdVoltage(int ch){ return adIn[ch] * adVoltPerDigit; }
	///	AD�̐��l
	int AdDigit(int ch){ return adIn[ch]; }

	///	��Ԃ̍X�V
	virtual void Update();
	void Read();
	void Write();
};

} //namespace Spr

#endif // SPR_DRIHCSh4_H
