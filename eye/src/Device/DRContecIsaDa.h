#ifndef VR_DRCONTECISADA_H
#define VR_DRCONTECISADA_H

#include "DRRealDevice.h"
#include "DVDaBase.h"

namespace Spr {

///	Contec��ISA D/A�J�[�h�p�̃h���C�o.
class SPR_DLL DRContecIsaDa:public DRRealDevice{
public:
	///	���z�f�o�C�X
	class VirtualDevice:public DVDaBase{
	protected:
		int ch;
		DRContecIsaDa* realDevice;
		char name[100];
	public:
		VirtualDevice(DRContecIsaDa* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->Voltage(ch, v); }
		virtual void Digit(int d){ realDevice->Digit(ch, d); }
		virtual const char* Name() const{ return name; }
	};
protected:
	char name[100];
	///	D/A��IO�A�h���X
	int address;
public:
	/**	�R���X�g���N�^
		@param address		D/A��I/O�A�h���X	*/
	DRContecIsaDa(int address);
	///	�f�o�C�X�̖��O
	virtual const char* Name() const { return name; }
	///	������
	virtual bool Init();
	///	���z�f�o�C�X�̓o�^
	virtual void Register(DVVirtualDevicePool& vpool);

	///	�d���o��
	void Voltage(int ch, float v);
	///	�d���o��(���l�w��)
	void Digit(int ch, int v);
};


}	//	namespace Spr

#endif