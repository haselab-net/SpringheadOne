#ifndef DEVICE_DRNITTAFORCE_H
#define DEVICE_DRNITTAFORCE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DRRealDevice.h"
#include "DVForceBase.h"
#include <base/Affine.h>

namespace Spr {


class SPR_DLL DRNittaForce: public DRRealDevice{
public:
	///	���z�f�o�C�X
	class VirtualDevice:public DVForceBase{
	protected:
		DRNittaForce* realDevice;
	public:
		VirtualDevice(DRNittaForce* r):realDevice(r){}
		virtual DRRealDevice* RealDevice() { return realDevice; }
		virtual const char* Name() const{ return realDevice->Name(); }
		virtual int GetDOF() const { return 6; }
		virtual float GetForce(int ch) const { return realDevice->GetForce(ch); }
	};

	enum{
		count1_addr			=	0xe8,
		count2_addr			=	0xe9,
		count3_addr			=	0xea,
		count4_addr			=	0xeb,
		count5_addr			=	0xec,
		count6_addr			=	0xed,
		force_units_addr	=	0xfc,
		full_fx_addr		=	0x80,
		force_addr			=	0x90,
		fx0_addr			=	0x90,
		full_fy_addr		=	0x81,
		fy0_addr			=	0x91,
		full_fz_addr		=	0x82,
		fz0_addr			=	0x92,
		full_mx_addr		=	0x83,
		mx0_addr			=	0x93,
		full_my_addr		=	0x84,
		my0_addr			=	0x94,
		full_mz_addr		=	0x85,
		mz0_addr			=	0x95,
		software_ver_no_addr=	0xf5,
		serial_no_addr		=	0xf8,
		model_no_addr		=	0xf9,
		DOF					=	6
	};
protected:
	char name[100];
	unsigned int baseAdr;
	unsigned short software_ver_no;
	unsigned short serial_no;
	unsigned short model_no;
	unsigned short full_range[DOF];
	unsigned short units;
	short force[DOF];

public:
	///
	DRNittaForce(int adr);
	///
	virtual ~DRNittaForce();
	///	������
	virtual bool Init();
	///	�o�^
	virtual void Register(DVVirtualDevicePool& vpool);
	///	���O
	virtual const char* Name() const { return name; }
	///	�{�[�h��̃^�C�}�[�̓ǂݏo��
	unsigned short GetCounter2K();
	///	�{�[�h��̃^�C�}�[�̓ǂݏo��
	unsigned short GetCounter8K();
	///	�^�C�}�[�Ȃǂ̃��Z�b�g
	void Reset();
	///	�͂̓ǂݏo��
	float GetForce(int ch){
		if (ch > DOF-1) ch = DOF-1;
		if (ch < 0) ch = 0;
		ReadForce(ch);
		return Force(ch);
	}
protected:
	///	�f�[�^���j���[�g���ɕϊ�
	float DRNittaForce::Force(int num){
		return (float)force[num]/(float)16384*(float)full_range[num];
	}
	///	�͂̎擾
	void ReadForce(int ch);
	///	���W�X�^�̓ǂݏo��
	void ReadReg(unsigned short a, unsigned short* d);
	void WriteReg(unsigned short a, unsigned short d);
};

}	//	namespace Spr

#endif
