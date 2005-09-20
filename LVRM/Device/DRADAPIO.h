#ifndef VR_DRCONTECISADA_H
#define VR_DRCONTECISADA_H

#include "DRRealDevice.h"
#include "DVDaBase.h"
#include "DVAdBase.h"
#include "tuadapio.h"

namespace LVRM {

///	�^�[�g���H�Ƃ�A/D�ED/A�EPIO�p�̃h���C�o.
class DRAdapio:public DRRealDevice{
public:
	///	���z�f�o�C�X
	class DVDA:public DVDaBase{
	protected:
		int ch;
		DRAdapio* realDevice;
		char name[100];
	public:
		DVDA(DRAdapio* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual void Voltage(float v){ realDevice->DAVoltage(ch, v); }
		virtual const char* Name() const{ return name; }
	};
	class DVAD:public DVAdBase{
	protected:
		int ch;
		DRAdapio* realDevice;
		char name[100];
	public:
		DVAD(DRAdapio* r, int c);
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual float Voltage(){ return realDevice->ADVoltage(ch); }
		virtual const char* Name() const{ return name; }
	};
protected:
	char name[100];
	int id;
public:
	/**	�R���X�g���N�^
		@param id		ADAPIO����������ꍇ�C���Ԗڂ̃f�o�C�X�����w��D
	*/
	DRAdapio(int i=0);
	virtual ~DRAdapio();
	///	�f�o�C�X�̖��O
	virtual const char* Name() const { return name; }
	///	������
	virtual bool Init();
	///	���z�f�o�C�X�̓o�^
	virtual void Register(DVVirtualDevicePool& vpool);
	///	�d���o��
	void DAVoltage(int ch, float v);
	///	�d������
	float ADVoltage(int ch);
	/// �I������
	virtual void CloseDevice();
};


}	//	namespace LVRM

#endif
