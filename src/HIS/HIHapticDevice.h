#ifndef VR_HAPTIC_HDHAPTIC_H
#define VR_HAPTIC_HDHAPTIC_H
#include <Base/Env.h>
#include <Base/Affine.h>
#include <Base/BaseUtility.h>
#include <set>

namespace Spr {
class DRRealDevice;

#ifdef _MSC_VER
 #pragma warning(disable:4275)
 #pragma warning(disable:4251)
#endif

//----------------------------------------------------------------------------
//	HIHapticDevice
#define HI_DEVICE_TYPE_DEF(x)	\
static const char* GetTypeStatic(){ return x; }	\
virtual  const char* GetType(){ return x; }


///	�͊o�񎦑��u�̊��N���X
class SPR_DLL HIHapticDevice: public UTRefCount{
protected:
	static std::set<DRRealDevice*> realDevices;
	static int deviceUpdateStep;
	int updateStep;
	bool bGood;
public:
	/**@name	�������Ȃ�	*/
	//@{
	/// 
	HIHapticDevice(){ updateStep = deviceUpdateStep; bGood=false;}
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
	virtual void Update(float dt);
	///	
	bool IsGood(){ return bGood; }
	///	�f�o�C�X�̃^�C�v
	HI_DEVICE_TYPE_DEF("HapticDevice")
};

//----------------------------------------------------------------------------
//	HIForceDevice3D

///	���i��]6���R�x�̃f�o�C�X
class SPR_DLL HIForceDevice6D: public HIHapticDevice{
protected:
	float alpha;
	Vec3f vel, angVel;
	Vec3f lastPos;
	Quaternionf lastOri;
public:
	///
	HIForceDevice6D();
	///
	virtual ~HIForceDevice6D(){}
	virtual void Update(float dt);

	/**@name	�f�o�C�X�̏��	*/
	//@{
	///	�f�o�C�X�̌��݈ʒu��Ԃ�
	virtual Vec3f GetPos()=0;
	///	�f�o�C�X�̌�����Ԃ�
	virtual Quaternionf GetOri()=0;
	///	�f�o�C�X�̑��x��Ԃ�
	virtual Vec3f GetVel(){ return vel; }
	///	�f�o�C�X�̊p���x��Ԃ�
	virtual Vec3f GetAngVel(){ return angVel; }
	///	�f�o�C�X�̎��ۂ̒񎦃g���N��Ԃ�
	virtual Vec3f GetTorque()=0;
	///	�f�o�C�X�̎��ۂ̒񎦗͂�Ԃ�
	virtual Vec3f GetForce()=0;
	///	�f�o�C�X�̖ڕW�o�͂ƃg���N�o�͂�ݒ肷��
	virtual void SetForce(const Vec3f& f, const Vec3f& t)=0;
	//@}
};

/*class SPR_DLL HIForceDevice3DPlus: public HIForceDevice3D{
	virtual Vec3f GetRot()=0;
};
*/

//----------------------------------------------------------------------------
//	HIForceDevice3D
///	���i3���R�x�̃f�o�C�X
class SPR_DLL HIForceDevice3D: public HIForceDevice6D{
public:
	HIForceDevice3D();
	///	�f�o�C�X�̖ڕW�͏o�͂�ݒ肷��
	virtual void SetForce(const Vec3f& f)=0;
	virtual void SetForce(const Vec3f& f, const Vec3f& t){ SetForce(f); }
	///	�f�o�C�X�̎��ۂ̒񎦃g���N��Ԃ�
	virtual Vec3f GetTorque(){ return Vec3f(); }
	virtual Quaternionf GetOri(){ return Quaternionf(); } 
};

//----------------------------------------------------------------------------
//	HIForceDevice7D
///	7���R�x�̃f�o�C�X
class SPR_DLL HIForceDevice7D: public HIForceDevice6D{
public:
	virtual ~HIForceDevice7D(){}
	///	�f�o�C�X�̔c��������Ԃ�
	virtual float GetGap()=0;
	///	�f�o�C�X�̒񎦔c���͂�Ԃ�
	virtual float GetGrasp()=0;
	///	�f�o�C�X�̖ڕW�͔c���͂�ݒ肷��
	virtual void SetGrasp(float g)=0;
};

}	//	namespace Spr
#endif
