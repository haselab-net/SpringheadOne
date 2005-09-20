#ifndef SPIDARH_VM_H
#define SPIDARH_VM_H
//SPIDAR-H
/////////////////////////////////////////////
#include <His/HISpidar4quad.h> //LVRM
#include <Device/DRUsb20Simple.h>
//////////////////////////////////////
//#include <HIS/HISpidar4.h> //spring head
//////////////////////////////////////////
// ����m�FSPIDAR�p ���[�^�̎��t�����W
#define PX	0.3125f			//	x�����̕ӂ̒���/2 [m]
#define PY	0.23f			//	y�����̕ӂ̒���/2 [m]
#define PZ	0.265f			//	z�����̕ӂ̒���/2 [m]
#define MAXON	0.3776f
#define LPP		2.924062107079e-5f
const float K = 10;	// 5�キ 10����
#define SpidarUpdateRate	2

namespace LVRM {;

/**	Spidar-H
	�t���[���̃T�C�Y�⃂�[�^�̕������܂�	*/

class SpidarH_VM: public HISpidar4quad{

public:
	///	�f�o�C�X�̃^�C�v
//	HI_DEVICE_TYPE_DEF("Spidar4VM")

	DVDeviceManager devMan;
	HISpidar4quad h_spidar[2];
	SpidarH_VM();
	virtual bool Init();
	virtual void SetMinForce(int idx);

	virtual Vec3f GetForce(int idx);
	virtual Vec3f GetPos(int idx);
		
	virtual void Update(int idx);
	//SPIDAR-H
/////////////////////////////////////
  	bool bMinForce;
	// ��ԑJ�ڃJ�E���^
 	void reset_spidar();
    void spidarCheck();

	Vec3f convertEuclidToPole(Vec3f euclid);
	Vec3f convertPoleToEuclid(Vec3f pole);
};

}	//	namespace
#endif
