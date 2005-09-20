#ifndef VR_HAPTIC_HDHAPTIC_H
#define VR_HAPTIC_HDHAPTIC_H
#include "../../../LVRM/Base/Affine.h"
#include <set>

namespace LVRM {
class DRRealDevice;


#pragma warning(disable:4275)
#pragma warning(disable:4251)

//----------------------------------------------------------------------------
//	HIHapticDevice

///	�͊o�񎦑��u�̊��N���X
class HIHapticDevice{
protected:
	static std::set<DRRealDevice*> realDevices;
	static int deviceUpdateStep;
	int updateStep;
public:
	/**@name	�������Ȃ�	*/
	//@{
	/// 
	HIHapticDevice(){ updateStep = deviceUpdateStep; }
	virtual ~HIHapticDevice(){}
	///	�L�����u���[�V�����̑O�ɍs������
	virtual bool BeforeCalib(){ return true; }
	///	�L�����u���[�V��������
	virtual bool Calib(){ return true; }
	///	�L�����u���[�V�����̂��Ƃɍs������
	virtual bool AfterCalib(){ return true; }
	///
	void AddRealDeviceDependency(DRRealDevice* rd);
	//@}
	/**	�f�o�C�X�̏�Ԃ��X�V����D	*/
	virtual void Update();
};

//----------------------------------------------------------------------------
//	HDForceDevice3D

///	���i3���R�x�̃f�o�C�X
class HDForceDevice3D: public HIHapticDevice{
protected:
public:
	/// 
	HDForceDevice3D(){}
	///
	virtual ~HDForceDevice3D(){}
	
	/**@name	�f�o�C�X�̏��	*/
	//@{
	///	�f�o�C�X�̌��݈ʒu��Ԃ�
	virtual Vec3f GetPos()=0;
	///	�f�o�C�X�̖ڕW�͏o�͂�Ԃ�	
	virtual Vec3f GetForce()=0;
	///	�f�o�C�X�̖ڕW�͏o�͂�ݒ肷��
	virtual void SetForce(const Vec3f& f)=0;
	//@}
};


//----------------------------------------------------------------------------
//	HDForceDevice6D
///	6���R�x�̃f�o�C�X
class HDForceDevice6D: public HDForceDevice3D{
public:
	virtual ~HDForceDevice6D(){}
	///	�f�o�C�X�̎p����Ԃ�
	virtual Matrix3f GetRot()=0;

	///	�f�o�C�X�̒񎦃g���N��Ԃ�	
	virtual Vec3f GetTorque()=0;

	///	�f�o�C�X�̖ڕW�g���N�o�͂�ݒ肷��
	virtual void SetTorque(const Vec3f& a)=0;
};

//----------------------------------------------------------------------------
//	HDForceDevice7D
///	7���R�x�̃f�o�C�X
class HDForceDevice7D: public HDForceDevice6D{
public:
	virtual ~HDForceDevice7D(){}
	
	///	�f�o�C�X�̔c��������Ԃ�
	virtual float GetGap()=0;
	///	�f�o�C�X�̒񎦔c���͂�Ԃ�
	virtual float GetGrasp()=0;
	///	�f�o�C�X�̖ڕW�͔c���͂�ݒ肷��
	virtual void SetGrasp(float g)=0;
};

}	//	namespace LVRM
#endif
