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

///	力覚提示装置の基底クラス
class HIHapticDevice{
protected:
	static std::set<DRRealDevice*> realDevices;
	static int deviceUpdateStep;
	int updateStep;
public:
	/**@name	初期化など	*/
	//@{
	/// 
	HIHapticDevice(){ updateStep = deviceUpdateStep; }
	virtual ~HIHapticDevice(){}
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalib(){ return true; }
	///	キャリブレーション処理
	virtual bool Calib(){ return true; }
	///	キャリブレーションのあとに行う処理
	virtual bool AfterCalib(){ return true; }
	///
	void AddRealDeviceDependency(DRRealDevice* rd);
	//@}
	/**	デバイスの状態を更新する．	*/
	virtual void Update();
};

//----------------------------------------------------------------------------
//	HDForceDevice3D

///	並進3自由度のデバイス
class HDForceDevice3D: public HIHapticDevice{
protected:
public:
	/// 
	HDForceDevice3D(){}
	///
	virtual ~HDForceDevice3D(){}
	
	/**@name	デバイスの状態	*/
	//@{
	///	デバイスの現在位置を返す
	virtual Vec3f GetPos()=0;
	///	デバイスの目標力出力を返す	
	virtual Vec3f GetForce()=0;
	///	デバイスの目標力出力を設定する
	virtual void SetForce(const Vec3f& f)=0;
	//@}
};


//----------------------------------------------------------------------------
//	HDForceDevice6D
///	6自由度のデバイス
class HDForceDevice6D: public HDForceDevice3D{
public:
	virtual ~HDForceDevice6D(){}
	///	デバイスの姿勢を返す
	virtual Matrix3f GetRot()=0;

	///	デバイスの提示トルクを返す	
	virtual Vec3f GetTorque()=0;

	///	デバイスの目標トルク出力を設定する
	virtual void SetTorque(const Vec3f& a)=0;
};

//----------------------------------------------------------------------------
//	HDForceDevice7D
///	7自由度のデバイス
class HDForceDevice7D: public HDForceDevice6D{
public:
	virtual ~HDForceDevice7D(){}
	
	///	デバイスの把持距離を返す
	virtual float GetGap()=0;
	///	デバイスの提示把持力を返す
	virtual float GetGrasp()=0;
	///	デバイスの目標力把持力を設定する
	virtual void SetGrasp(float g)=0;
};

}	//	namespace LVRM
#endif
